/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#pragma once

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/container/flat_set.hpp>
#include <memory>
#include <sys/stat.h>
#include <folly/json/dynamic.h>

#include "hphp/runtime/base/config.h"
#include "hphp/runtime/base/configs/repo-options-flags-generated.h"
#include "hphp/runtime/base/package.h"
#include "hphp/runtime/base/typed-value.h"
#include "hphp/runtime/base/types.h"

#include "hphp/util/compilation-flags.h"
#include "hphp/util/configs/autoload.h"
#include "hphp/util/configs/hacklang.h"
#include "hphp/util/configs/jit.h"
#include "hphp/util/configs/php7.h"
#include "hphp/util/configs/server.h"
#include "hphp/util/functional.h"
#include "hphp/util/hash-map.h"
#include "hphp/util/sha1.h"

#include "hphp/hack/src/parser/ffi_bridge/parser_ffi.rs.h"

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

struct AccessLogFileData;
struct ErrorLogFileData;
struct VirtualHost;
struct IpBlockMap;
struct SatelliteServerInfo;
struct FilesMatch;
struct Hdf;
struct IniSettingMap;

constexpr const char* kPackagesToml = "PACKAGES.toml";

using StringToIntMap = std::unordered_map<std::string, int>;

enum class JitSerdesMode {
  // NB: if changing the encoding here, make sure to update isJitSerializing()
  // and isJitDeserializing() as needed.
  //
  // Bit 0: serialize
  // Bit 1: deserialize
  Off                   = 0x0,
  Serialize             = 0x1,          // 00001
  SerializeAndExit      = 0x5,          // 00101
  Deserialize           = 0x2,          // 00010
  DeserializeOrFail     = 0x6,          // 00110
  DeserializeOrGenerate = 0xa,          // 01010
  DeserializeAndDelete  = 0xe,          // 01110
  DeserializeAndExit    = 0x12,         // 10010
};

enum class RepoMode {
  Closed    = 0,
  ReadOnly  = 1,
  ReadWrite = 2,
};

namespace hackc {
  struct NativeEnv;
  struct HhbcFlags;
  struct ParserFlags;
  struct DeclParserConfig;
}

namespace Facts {
// SQLFacts version number representing the DB's schema.  This number is
// determined randomly, but should match the number in the SQL Facts
// implementation.  We use this when we make a change that invalidates
// the cache, such as adding a new table which would otherwise be
// unpopulated without a cache rebuild.
constexpr size_t kSchemaVersion = 1916337637;
}

/*
 * The bare RepoOptions information that the parser cares about.
 */
struct RepoOptionsFlags {

#define S(name) friend Cfg::name;
SECTIONS_FOR_REPOOPTIONSFLAGS()
#undef S

  const PackageInfo& packageInfo() const { return m_packageInfo; }
  const SHA1& cacheKeySha1() const { return m_sha1; }

  ParserEnv getParserEnvironment() const;
  void initDeclConfig(hackc::DeclParserConfig&) const;
  void initHhbcFlags(hackc::HhbcFlags&) const;
  void initParserFlags(hackc::ParserFlags&) const;
  void initAliasedNamespaces(hackc::NativeEnv&) const;

  std::string autoloadQuery() const { return Query; }
  folly::dynamic autoloadQueryObj() const { return m_cachedQuery; }
  std::string trustedDBPath() const { return TrustedDBPath; }
  const std::vector<std::string>& autoloadRepoBuildSearchDirs() const {
    return RepoBuildSearchDirs;
  }

  /**
   * Allowlist consisting of the attributes, marking methods, which Facts
   * should index
   */
  const Cfg::StringVector& indexedMethodAttributes() const {
    return IndexedMethodAttributes;
  }

  // NB: Everything serialized here affects the cache for RE. Do not
  // put anything unnecessary or that changes spuriously.
  template <typename SerDe> void serde(SerDe& sd) {
    #define C(t, n) sd(n);
    CONFIGS_FOR_REPOOPTIONSFLAGS()
    #undef C

    sd(m_packageInfo);
    sd(m_sha1);
    sd(m_factsCacheBreaker);

    if constexpr (SerDe::deserializing) calcCachedQuery();
  }

  template <typename SerDe>
  static RepoOptionsFlags makeForSerde(SerDe& sd) {
    RepoOptionsFlags f;
    sd(f);
    return f;
  }

  const std::string& getFactsCacheBreaker() const { return m_factsCacheBreaker;}
  void calcCachedQuery();

private:
  RepoOptionsFlags() = default;

  #define C(t, n) t n;
  CONFIGS_FOR_REPOOPTIONSFLAGS()
  #undef C

  PackageInfo m_packageInfo;

  SHA1 m_sha1;
  std::string m_factsCacheBreaker;

  // The query to be used for autoloading
  folly::dynamic m_cachedQuery;

  friend struct RepoOptions;
};

namespace Stream {
struct Wrapper;
}

struct RepoOptionStats {
  RepoOptionStats() = default;
  RepoOptionStats(const std::string&, Stream::Wrapper*);
  bool missing() const {
    return !m_configStat.has_value() && !m_packageStat.has_value();
  }

  Optional<struct stat> m_configStat;
  Optional<struct stat> m_packageStat;
};

/*
 * RepoOptionsFlags plus extra state
 */
struct RepoOptions {
  RepoOptions(const RepoOptions&) = default;
  RepoOptions(RepoOptions&&) = default;

  const RepoOptionsFlags& flags() const { return m_flags; }
  const PackageInfo& packageInfo() const { return flags().packageInfo(); }
  const std::filesystem::path& path() const { return m_path; }
  const RepoOptionStats& stat() const { return m_stat; }

  const std::filesystem::path& dir() const { return m_repo; }
  const std::filesystem::path& autoloadDB() const { return m_autoloadDB; }

  bool operator==(const RepoOptions& o) const {
    // If we have hash collisions of unequal RepoOptions, we have
    // bigger problems.
    return m_flags.m_sha1 == o.m_flags.m_sha1;
  }
  bool operator!=(const RepoOptions& o) const { return !(*this == o); }

  static const RepoOptions& defaults();
  static const RepoOptions& defaultsForSystemlib();
  static void setDefaults(const Hdf& hdf, const IniSettingMap& ini);

  static const RepoOptions& forFile(const std::string& path);
private:
  RepoOptions() = default;
  RepoOptions(const char* str, const char* file);

  void filterNamespaces();
  void initDefaults(const Hdf& hdf, const IniSettingMap& ini);
  void initDefaultsForSystemlib();
  void calcCacheKey();
  void calcDynamic();
  void calcAutoloadDB();

  RepoOptionsFlags m_flags;

  // Path to .hhvmconfg.hdf
  std::filesystem::path m_path;
  RepoOptionStats m_stat;

  // Canonical path of repo root directory that contains .hhvmconfig.hdf
  std::filesystem::path m_repo;

  // The autoload DB specified by these repo options
  std::filesystem::path m_autoloadDB;

  bool m_init = false;

  static RepoOptions s_defaults;
  static RepoOptions s_defaultsForSystemlib;
};

/**
 * Configurable options set from command line or configurable file at startup
 * time.
 */
struct RuntimeOption {
  static void Load(
    IniSettingMap &ini, Hdf& config,
    const std::vector<std::string>& iniClis = std::vector<std::string>(),
    const std::vector<std::string>& hdfClis = std::vector<std::string>(),
    std::vector<std::string>* messages = nullptr,
    std::string cmd = "");

  static bool ServerExecutionMode() {
    return Cfg::Server::Mode;
  }

  static bool GcSamplingEnabled() {
    return EvalGCSampleRate > 0;
  }

  static bool JitSamplingEnabled() {
    return Cfg::Jit::Enabled && Cfg::Jit::SampleRate > 0;
  }

  static void ReadSatelliteInfo(
    const IniSettingMap& ini,
    const Hdf& hdf,
    std::vector<std::shared_ptr<SatelliteServerInfo>>& infos
  );

  static Optional<std::filesystem::path> GetHomePath(
    const folly::StringPiece user);

  static std::string GetDefaultUser();

  /**
   * Find a config file corresponding to the given user and parse its
   * settings into the given ini and hdf objects.
   *
   * Return true on success and false on failure.
   */
  static bool ReadPerUserSettings(const std::filesystem::path& confFileName,
                                  IniSettingMap& ini, Hdf& config);

  static std::string getTraceOutputFile();

  // Store the list of input values used to calculate the tier overwrites
  static std::map<std::string, std::string> TierOverwriteInputs;
  static void StoreTierOverwriteInputs(const std::string &machine, const std::string &tier,
    const std::string &task, const std::string &cpu, const std::string &tiers, const std::string &tags);

  static std::string BuildId;
  static std::string InstanceId;
  static std::string DeploymentId; // ID for set of instances deployed at once
  static int64_t ConfigId; // Queryable to verify a specific config was read
  static std::string PidFile;

  static std::map<std::string, ErrorLogFileData> ErrorLogs;
  static std::string LogFile;
  static std::string LogFileSymLink;
  static uint16_t LogFilePeriodMultiplier;

  static int LogHeaderMangle;
  static bool AlwaysEscapeLog;
  static bool AlwaysLogUnhandledExceptions;
  static bool NoSilencer;
  static int RuntimeErrorReportingLevel;
  static int ForceErrorReportingLevel; // Bitmask ORed with the reporting level

  static int RaiseDebuggingFrequency;
  static int64_t SerializationSizeLimit;

  static std::string AccessLogDefaultFormat;
  static std::map<std::string, AccessLogFileData> AccessLogs;

  static std::string AdminLogFormat;
  static std::string AdminLogFile;
  static std::string AdminLogSymLink;

  static std::map<std::string, AccessLogFileData> RPCLogs;

  static const std::string& GetServerPrimaryIPv4();
  static const std::string& GetServerPrimaryIPv6();
  static std::vector<std::shared_ptr<VirtualHost>> VirtualHosts;
  static std::shared_ptr<IpBlockMap> IpBlocks;
  static std::vector<std::shared_ptr<SatelliteServerInfo>>
         SatelliteServerInfos;

  /**
   * Legal root directory expressions in an include expression. For example,
   *
   *   include_once $PHP_ROOT . '/lib.php';
   *
   * Here, "$PHP_ROOT" is a legal include root. Stores what it resolves to.
   *
   *   RuntimeOption::IncludeRoots["$PHP_ROOT"] = "";
   *   RuntimeOption::IncludeRoots["$LIB_ROOT"] = "lib";
   */
  static std::map<std::string, std::string> IncludeRoots;

  static hphp_string_imap<std::string> StaticFileExtensions;
  static hphp_string_imap<std::string> PhpFileExtensions;
  static std::set<std::string> StaticFileGenerators;
  static std::vector<std::shared_ptr<FilesMatch>> FilesMatches;

  static std::string AdminServerIP;
  static int AdminServerPort;
  static int AdminThreadCount;
  static bool AdminServerEnableSSLWithPlainText;
  static bool AdminServerStatsNeedPassword;
  static std::string AdminPassword;
  static std::set<std::string> AdminPasswords;
  static std::set<std::string> HashedAdminPasswords;

  static std::string AdminDumpPath;

  static int  HttpDefaultTimeout;
  static int  HttpSlowQueryThreshold;

  static bool NativeStackTrace;
  static bool ServerErrorMessage;
  static bool RecordInput;
  static bool ClearInputOnSuccess;
  static std::string ProfilerOutputDir;
  static std::string CoreDumpEmail;
  static bool CoreDumpReport;
  static std::string CoreDumpReportDirectory;
  static std::string StackTraceFilename;
  static int StackTraceTimeout;
  static std::string RemoteTraceOutputDir;
  static std::set<std::string, stdltistr> TraceFunctions;

  static bool EnableStats;
  static bool EnableAPCStats;
  static bool EnableWebStats;
  static bool EnableMemoryStats;
  static bool EnableSQLStats;
  static bool EnableSQLTableStats;
  static bool EnableNetworkIOStatus;
  static std::string StatsXSL;
  static std::string StatsXSLProxy;
  static uint32_t StatsSlotDuration;
  static uint32_t StatsMaxSlot;
  static std::vector<std::string> StatsTrackedKeys;

  static int32_t ProfilerTraceBuffer;
  static double ProfilerTraceExpansion;
  static int32_t ProfilerMaxTraceBuffer;

  static int64_t MaxSQLRowCount;
  static int64_t SocketDefaultTimeout;

  static bool DisableSmallAllocator;

  static std::map<std::string, std::string> ServerVariables;

  static std::map<std::string, std::string> EnvVariables;

  // Eval options
  static bool EnableXHP;
  static bool CheckSymLink;
  static bool TrustAutoloaderPath;
  static bool EnableArgsInBacktraces;
  static bool EnableZendIniCompat;
  static bool TimeoutsUseWallTime;
  static bool EvalAuthoritativeMode;
  static int CheckCLIClientCommands;
  static JitSerdesMode EvalJitSerdesMode;
  static int ProfDataTTLHours;
  static std::string EvalJitSerdesFile;
  static std::string ProfDataTag;
  static bool DumpPreciseProfData;

  static std::string WatchmanRootSocket;
  static std::string WatchmanDefaultSocket;

  static hphp_string_map<TypedValue> ConstantFunctions;

  static std::vector<std::string> TzdataSearchPaths;

  static hphp_fast_string_set ActiveExperiments;
  static hphp_fast_string_set InactiveExperiments;

#define EVALFLAGS()                                                     \
  /* F(type, name, defaultVal) */                                       \
  /*                                                                    \
   * Maximum number of elements on the VM execution stack.              \
   */                                                                   \
  F(uint64_t, VMStackElms, kEvalVMStackElmsDefault)                     \
  F(int, StackCheckLeafPadding, 100)                                    \
  F(bool, EnableAsyncJIT, false)                                        \
  F(int, AsyncJitWorkerThreads, 4)                                      \
  /*                                                                    \
   * Initial space reserved for the global variable environment (in     \
   * number of global variables).                                       \
   */                                                                   \
  F(uint32_t, VMInitialGlobalTableSize,                                 \
    kEvalVMInitialGlobalTableSizeDefault)                               \
  F(bool, RecordSubprocessTimes,       false)                           \
  F(bool, AllowHhas,                   false)                           \
  F(bool, GenerateDocComments,         true)                            \
  F(bool, DisassemblerDocComments,     true)                            \
  F(bool, DisassemblerPropDocComments, true)                            \
  F(bool, LoadFilepathFromUnitCache,   false)                           \
  F(bool, WarnOnSkipFrameLookup,       true)                            \
  /*                                                                    \
   *  0 - Code coverage cannot be enabled through request param         \
   *  1 - Code coverage can be enabled through request param            \
   *  2 - Code coverage enabled                                         \
   */                                                                   \
  F(uint32_t, EnableCodeCoverage,      0)                               \
  /*                                                                    \
   *  0 - Per-file coverage cannot be enabled through request param     \
   *  1 - Per-file coverage can be enabled through request param        \
   *  2 - Per-file coverage enabled                                     \
   */                                                                   \
  F(uint32_t, EnablePerFileCoverage, 0)                                 \
  F(bool, EnableFuncCoverage,          false)                           \
  /* The number of worker threads to spawn for facts extraction. */     \
  F(uint64_t, FactsWorkers,            Process::GetCPUCount())          \
  /* Whether the HackC compiler should inherit the compiler config of the
     HHVM process that launches it. */                                  \
  F(bool, HackCompilerInheritConfig,   true)                            \
  /* enable decl-directed bytecode compilation */                       \
  F(bool, EnableDecl, false)                                            \
  F(uint32_t, LogDeclDeps, 0)                                           \
  F(uint32_t, LogDeclErrors, 0)                                         \
  F(bool, LogAllDeclTearing, false)                                     \
  /* When using embedded data, extract it to the ExtractPath or the
   * ExtractFallback. */                                                \
  F(string, EmbeddedDataExtractPath,   "/var/run/hhvm_%{type}_%{buildid}") \
  F(string, EmbeddedDataFallbackPath,  "/tmp/hhvm_%{type}_%{buildid}_XXXXXX") \
  /* Whether to trust existing versions of extracted embedded data. */  \
  F(bool, EmbeddedDataTrustExtract,    true)                            \
  F(bool, LogThreadCreateBacktraces,   false)                           \
  F(bool, FailJitPrologs,              false)                           \
  F(bool, UseHHBBC,                    !getenv("HHVM_DISABLE_HHBBC"))   \
  /* Threshold number of units to log to hhvm_whole_program table.
     systemlib has around 200 units, so use a larger default to avoid
     logging for unit tests. */                                         \
  F(uint32_t, HHBBCMinUnitsToLog,      1000)                            \
  F(bool, CachePerRepoOptionsPath,     true)                            \
  F(bool, LogHackcMemStats,            false)                           \
  F(uint32_t, TsameCollisionSampleRate, 1)                              \
  /* CheckBuiltinParamTypeHints
   * 0 - Do not check parameter type hints of builtins
   * 1 - Treat builtin parameter type hints as <<__Soft>>
   * 2 - Enforce builtin parameter type hints
   */                                                                   \
  F(int32_t, CheckBuiltinParamTypeHints, 2)                             \
  /* Unused. Scheduled for removal: T175741557 */                       \
  F(int32_t, EnforceGenericsUB,        2)                               \
  /* WarnOnTooManyArguments:
   * 0 -> no warning, 1 -> warning, 2 -> exception
   */                                                                   \
  F(uint32_t, WarnOnTooManyArguments,  0)                               \
  /* GetClassBadArgument:
   * 0 -> no warning, 1 -> warning, 2 -> exception
   */                                                                   \
  F(uint32_t, GetClassBadArgument,     0)                               \
  /* WarnOnIncDecInvalidType:
   * 0 - No restrictions on types that can be incremented or decremented
   * 1 - Warn when incrementing or decrementing non numeric types
   * 2 - Throw when incrementing or decrementing non numeric types
   */                                                                   \
  F(uint32_t, WarnOnIncDecInvalidType, 0)                               \
  /* WarnOnImplicitCoercionOfEnumValue
   * This flag exists to control behaviour when implicit coercion is
   * taking place on an enum value.
   * 0 - No warning
   * 1 - Warning
   * 2 - Do not do implicit coercion
   */                                                                   \
  F(uint32_t, WarnOnImplicitCoercionOfEnumValue, 0)                     \
  F(bool, EnableLogBridge,             true)                            \
  F(bool, MoreAccurateMemStats,        true)                            \
  F(bool, MemInfoCheckCgroup2,         true)                            \
  F(bool, SpinOnCrash,                 false)                           \
  F(uint32_t, DumpRingBufferOnCrash,   0)                               \
  F(bool, PerfPidMap,                  true)                            \
  F(bool, PerfPidMapIncludeFilePath,   true)                            \
  F(bool, PerfJitDump,                 false)                           \
  F(string, PerfJitDumpDir,            "/tmp")                          \
  F(bool, PerfDataMap,                 false)                           \
  F(bool, KeepPerfPidMap,              false)                           \
  F(uint32_t, ThreadTCMainBufferSize,  6 << 20)                         \
  F(uint32_t, ThreadTCColdBufferSize,  6 << 20)                         \
  F(uint32_t, ThreadTCFrozenBufferSize,4 << 20)                         \
  F(uint32_t, ThreadTCDataBufferSize,  256 << 10)                       \
  F(uint32_t, RDSSize,                 64 << 20)                        \
  F(uint32_t, HHBCArenaChunkSize,      10 << 20)                        \
  F(bool, ProfileBC,                   false)                           \
  F(bool, ProfileHeapAcrossRequests,   false)                           \
  F(bool, ProfileHWEnable,             true)                            \
  F(string, ProfileHWEvents,           std::string(""))                 \
  F(bool, ProfileHWExcludeKernel,      false)                           \
  F(bool, ProfileHWFastReads,          false)                           \
  F(bool, ProfileHWStructLog,          false)                           \
  F(int32_t, ProfileHWExportInterval,  30)                              \
  F(string, ReorderProps,              reorderPropsDefault())           \
  F(bool, ReorderRDS,                  true)                            \
  F(double, RDSReorderThreshold,       0.0005)                          \
  F(uint32_t, ProfileGlobalsLimit,     200)                             \
  F(double, ProfileGlobalsSlowExitThreshold, 0.98)                      \
  F(uint32_t, GdbSyncChunks,           128)                             \
  F(bool, TraceCommandLineRequest,     true)                            \
  F(bool, EmitDebuggerIntrCheck,       true)                            \
  /* Log the profile used to optimize array-like gets and sets. */      \
  F(bool, LogArrayAccessProfile,      false)                            \
  F(double, CoeffectFunParamProfileThreshold, 0.10)                     \
  F(bool, AssemblerFoldDefaultValues,  true)                            \
  F(uint64_t, AssemblerMaxScalarSize,  2147483648) /* 2GB */            \
  F(uint64_t, FuncCountHint,           10000)                           \
  F(uint64_t, PGOFuncCountHint,        1000)                            \
  F(bool, RegionRelaxGuards,           true)                            \
  /* DumpBytecode =1 dumps user php, =2 dumps systemlib & user php */   \
  F(int32_t, DumpBytecode,             0)                               \
  /* DumpHhas =1 dumps user php, =2 dumps systemlib & user php */       \
  F(int32_t, DumpHhas,                 0)                               \
  F(string, DumpHhasToFile,            "")                              \
  F(bool, DumpTC,                      false)                           \
  F(string, DumpTCPath,                "/tmp")                          \
  F(bool, DumpTCAnchors,               false)                           \
  F(uint32_t, DumpIR,                  0)                               \
  F(uint32_t, DumpIRJson,             0)                               \
  F(bool, DumpTCAnnotationsForAllTrans,debug)                           \
  /* DumpInlDecision 0=none ; 1=refuses ; 2=refuses+accepts */          \
  F(uint32_t, DumpInlDecision,         0)                               \
  F(uint32_t, DumpRegion,              0)                               \
  F(bool,     DumpCallTargets,         false)                           \
  F(bool,     DumpLayoutCFG,           false)                           \
  F(bool,     DumpHHIRInLoops,         false)                           \
  F(bool,     DumpVBC,                 false)                           \
  F(bool,     DumpArrAccProf,          false)                           \
  F(bool,     DumpCoeffectFunParamProf,false)                           \
  F(bool, DumpAst,                     false)                           \
  F(bool, DumpTargetProfiles,          false)                           \
  F(bool, DumpJitProfileStats,         false)                           \
  F(bool, DumpJitEnableRenameFunctionStats, false)                      \
  F(bool, MapTgtCacheHuge,             false)                           \
  F(bool, NewTHPHotText,               false)                           \
  F(bool, FileBackedColdArena,         useFileBackedArenaDefault())     \
  F(string, ColdArenaFileDir,          "/tmp")                          \
  F(uint32_t, MaxHotTextHugePages,     hotTextHugePagesDefault())       \
  F(uint32_t, MaxLowMemHugePages,      hugePagesSoundNice() ? 8 : 0)    \
  F(uint32_t, MaxHighArenaHugePages,   0)                               \
  F(uint32_t, Num1GPagesForReqHeap,    0)                               \
  F(uint32_t, Num2MPagesForReqHeap,    0)                               \
  F(uint32_t, NumReservedSlabs,        0)                               \
  F(uint32_t, NumReservedMBForSlabs,   0)                               \
  F(uint32_t, Num1GPagesForA0,         0)                               \
  F(uint32_t, Num2MPagesForA0,         0)                               \
  F(bool, BigAllocUseLocalArena,       true)                            \
  F(bool, JsonParserUseLocalArena,     true)                            \
  F(bool, XmlParserUseLocalArena,      true)                            \
  F(bool, LowStaticArrays,             (!use_lowptr ||                  \
                                        !ServerExecutionMode()))        \
  F(bool, RecycleAProf,                true)                            \
  F(int64_t, HeapPurgeWindowSize,      5 * 1000000)                     \
  F(uint64_t, HeapPurgeThreshold,      128 * 1024 * 1024)               \
  /* GC Options: See heap-collect.cpp for more details */               \
  F(bool, EagerGC,                     eagerGcDefault())                \
  F(bool, FilterGCPoints,              true)                            \
  F(bool, Quarantine,                  eagerGcDefault())                \
  F(bool, SanitizeReqHeap,             false)                           \
  F(bool, HeapAllocSampleNativeStack,  false)                           \
  F(bool, LogKilledRequests,           true)                            \
  F(uint32_t, GCSampleRate,            0)                               \
  F(uint32_t, HeapAllocSampleRequests, 0)                               \
  F(uint32_t, HeapAllocSampleBytes,    256 * 1024)                      \
  F(uint32_t, SlabAllocAlign,          64)                              \
  F(uint32_t, MemTrackStart,           3500)                            \
  F(uint32_t, MemTrackEnd,             3700)                            \
  F(int64_t, GCMinTrigger,             64L<<20)                         \
  F(double, GCTriggerPct,              0.5)                             \
  F(bool, TwoPhaseGC,                  false)                           \
  F(bool, EnableGC,                    enableGcDefault())               \
  /* End of GC Options */                                               \
  F(bool, Verify,                      getenv("HHVM_VERIFY"))           \
  F(bool, VerifyOnly,                  false)                           \
  F(bool, FatalOnVerifyError,          !RepoAuthoritative)              \
  F(bool, AbortBuildOnVerifyError,     true)                            \
  F(bool, AbortBuildOnCompilerError,   true)                            \
  F(bool, VerifySystemLibHasNativeImpl, true)                           \
  F(uint32_t, StaticContentsLogRate,   100)                             \
  F(uint32_t, LogUnitLoadRate,         0)                               \
  F(uint32_t, MaxDeferredErrors,       50)                              \
  F(uint32_t, SerDesSampleRate,            0)                           \
  F(int, SimpleJsonMaxLength,        2 << 20)                           \
  F(uint32_t, TraceServerRequestRate,      0)                           \
  /* Tracing Options */                                                 \
  /* Base tracing sample rate for all requests */                       \
  F(uint32_t, TracingSampleRate,              0)                        \
  /* Tracing sample rate for first N requests */                        \
  F(uint32_t, TracingPerRequestCount,         0)                        \
  F(uint32_t, TracingPerRequestSampleRate,    0)                        \
  /* Tracing sample rate for first N requests per URL */                \
  F(uint32_t, TracingFirstRequestsCount,      0)                        \
  F(uint32_t, TracingFirstRequestsSampleRate, 0)                        \
  /* Empty string disables any Artillery tracing */                     \
  F(std::string, ArtilleryTracePolicy, "")                              \
  /* Opaque tag to add to each trace. Useful for aggregation */         \
  F(std::string, TracingTagId, "")                                      \
  F(bool, EnableCompactBacktrace, true)                                 \
  F(bool, EnableNuma, (numa_num_nodes > 1) && ServerExecutionMode())    \
  F(bool, EnableCallBuiltin, true)                                      \
  F(bool, EnableReusableTC,   reuseTCDefault())                         \
  F(bool, LogServerRestartStats, false)                                 \
  /* Extra bytes added to each area (Hot/Cold/Frozen) of a translation. \
   * If we don't end up using a reusable TC, we'll drop the padding. */ \
  F(uint32_t, ReusableTCPadding, 128)                                   \
  F(int64_t,  StressUnitCacheFreq, 0)                                   \
  /* Perf warning sampling rates. The SelectHotCFG warning is noisy. */ \
  F(int64_t, PerfWarningSampleRate, 1)                                  \
  F(int64_t, SelectHotCFGSampleRate, 100)                               \
  F(int64_t, FunctionCallSampleRate, 0)                                 \
  F(double, InitialLoadFactor, 1.0)                                     \
  /* Controls emitting checks for bespoke arrays and using logging      \
   * arrays at runtime.                                                 \
   *                                                                    \
   * 0 - Disable bespokes. We assume that all array-likes have their    \
   *     standard (aka "vanilla") layouts.                              \
   * 1 - Test bespokes. We emit checks for vanilla layouts and produce  \
   *     logging arrays based on the request ID. If rid % 2 == 1, then  \
   *     a logging array is generated.                                  \
   * 2 - Production bespokes. We emit checks as in (1), and produce     \
   *     logging arrays based on per creation site sampling with the    \
   *     sample rate specified by EmitLoggingArraySampleRate. If the    \
   *     sample rate is 0, logging arrays are never constructed.        \
   *     Logging arrays are only created before RTA has begun. */       \
  F(int32_t, BespokeArrayLikeMode, 2)                                   \
  F(uint64_t, BespokeEscalationSampleRate, 0)                           \
  F(uint64_t, EmitLoggingArraySampleRate, 17)                           \
  F(string, ExportLoggingArrayDataPath, "")                             \
  /* Should we use structs?                                             \
   * If so, how many layouts and how big can they get?                  \
   */                                                                   \
  F(bool, EmitBespokeStructDicts, true)                                 \
  F(uint16_t, BespokeMaxNumStructLayouts, 1 << 14)                      \
  /* Do not use! Use StructLayout::maxNumKeys instead */                \
  F(uint16_t, BespokeStructDictMaxNumKeys, 2048)                        \
  F(double, BespokeStructDictKeyCoverageThreshold, 95.0)                \
  F(uint8_t, BespokeStructDictMinKeys, 128)                             \
  F(double, BespokeStructDictMaxSizeRatio, 2.0)                         \
  /* What is the maximum number of keys to track in key order           \
   * profiles? */                                                       \
  F(uint64_t, BespokeMaxTrackedKeys, 2048)                              \
  F(bool, EmitAPCBespokeArrays, true)                                   \
  /* Should we use monotypes? */                                        \
  F(bool, EmitBespokeMonotypes, false)                                  \
  F(int64_t, ObjProfMaxNesting, 2000)                                   \
  /* Choice of layout selection algorithms:                             \
   *                                                                    \
   * 0 - Default layout selection algorithm based on profiling.         \
   *     May use a mix of vanilla and bespoke array-likes.              \
   * 1 - Specialize all sources and sinks on vanilla layouts.           \
   * 2 - Specialize sources on vanilla, but sinks on top. */            \
  F(int32_t, BespokeArraySpecializationMode, 0)                         \
  /* We will use specialized layouts for a given array if they cover    \
   * the given percent of operations logged during profiling.           \
   *                                                                    \
   * We can generate code for a bespoke sink in three ways:             \
   *  1. We can do "top codegen" that handles any array layout.         \
   *  2. We can specialize layouts and fall back to top codegen.        \
   *  3. We can specialize layouts and side-exit on guard failure.      \
   *                                                                    \
   * We use a couple heuristics to choose between these options. If we  \
   * see one layout that covers `SideExitThreshold` percent cases, and  \
   * we saw at most `SideExitMaxSources` sources reach this sink, with  \
   * at least `SideExitMinSampleCount` samples each, we'll side-exit.   \
   *                                                                    \
   * Else, if multiple layouts cover SpecializationThreshold and at at  \
   * least one of them covers SpecializationMinThreshold we will        \
   * specialize to both layouts and fall back to top codegen. If one    \
   * layout covers `SpecializationThreshold` percent, we will           \
   * specialize and fall back to top codegen. Otherwise, we'll do top   \
   * codegen. */                                                        \
  F(double, BespokeArraySourceSpecializationThreshold, 95.0)            \
  F(double, BespokeArraySinkSpecializationThreshold,   95.0)            \
  F(double, BespokeArraySinkIteratorSpecializationThreshold, 92.0)      \
  F(double, BespokeArraySinkSpecializationMinThreshold, 85.0)           \
  F(double, BespokeArraySinkMultiLayoutThreshold, 0.999)                \
  F(double, BespokeArraySinkSideExitThreshold, 95.0)                    \
  F(uint64_t, BespokeArraySinkSideExitMaxSources, 64)                   \
  F(uint64_t, BespokeArraySinkSideExitMinSampleCount, 4)                \
  /* When this flag is on, var_export outputs d/varrays. */             \
  F(bool, HackArrDVArrVarExport, false)                                 \
  /* Raise a notice when the result of appending to a dict or darray    \
   * is affected by removing keys from that array-like. */              \
  F(bool, DictDArrayAppendNotices, true)                                \
  /* Warn if is expression are used with type aliases that cannot be    |
   * resolved */                                                        \
  F(bool, IsExprEnableUnresolvedWarning, false)                         \
  /* Raise a notice if a Class type is used as a memo key */            \
  F(bool, ClassMemoNotices, false)                                      \
  /*  Raise a notice if a ClsMeth type is passed to a function that
   *  expects a vec/varray */                                           \
  F(bool, VecHintNotices, false)                                        \
  /* Switches on miscellaneous junk. */                                 \
  F(bool, NoticeOnCreateDynamicProp, false)                             \
  F(bool, NoticeOnReadDynamicProp, false)                               \
  F(bool, NoticeOnImplicitInvokeToString, false)                        \
  F(bool, FatalOnConvertObjectToString, false)                          \
  /* When this flag is on, var_dump for
   * classes and lazy classes outputs string(...). */                   \
  F(bool, ClassAsStringVarDump, true)                                   \
  /* When this flag is on, var_export for
   * classes and lazy classes outputs a string. */                      \
  F(bool, ClassAsStringVarExport, false)                                \
  /* When this flag is on, gettype for
   * classes and lazy classes outputs string. */                        \
  F(bool, ClassAsStringPrintR, false)                                   \
  /* When this flag is on, print_r for
   * classes and lazy classes outputs a string. */                      \
  F(bool, ClassAsStringGetType, true)                                   \
  /* trigger E_USER_WARNING error when getClassName()/getMethodName()
   * is used on __SystemLib\MethCallerHelper */                         \
  F(bool, NoticeOnMethCallerHelperUse, false)                           \
  /*                                                                    \
   * Control dynamic calls to functions and dynamic constructs of       \
   * classes which haven't opted into being called that way.            \
   *                                                                    \
   * 0 - Do nothing                                                     \
   * 1 - Warn if meth_caller is apc serialized                          \
   * 2 - Throw exception if meth_caller is apc serialized               \
   */                                                                   \
  F(int32_t, ForbidMethCallerAPCSerialize, 0)                           \
  F(int32_t, ForbidMethCallerHelperSerialize, 0)                        \
  F(bool, NoticeOnMethCallerHelperIsObject, false)                      \
  F(bool, NoticeOnCollectionToBool, false)                              \
  F(bool, NoticeOnSimpleXMLBehavior, false)                             \
  /*                                                                    \
   * Don't allow unserializing to __PHP_Incomplete_Class                \
   * 0 - Nothing                                                        \
   * 1 - Warn                                                           \
   * 2 - Throw exception                                                \
   */                                                                   \
  F(int32_t, ForbidUnserializeIncompleteClass, 0)                       \
  /*                                                                    \
   * Map from coeffect name to enforcement level                        \
   * e.g. {'pure' => 2, 'rx' => 1}                                      \
   */                                                                   \
  F(StringToIntMap, CoeffectEnforcementLevels, coeffectEnforcementLevelsDefaults()) \
  F(uint64_t, CoeffectViolationWarningMax, std::numeric_limits<uint64_t>::max()) \
  /*                                                                    \
   * Enforce deployment boundaries.                                     \
   */                                                                   \
  F(bool, EnforceDeployment, true)                                     \
  F(uint32_t, DeploymentViolationWarningSampleRate, 1)                  \
  /*
   * Enforce top level and method level internal keyword                \
   * 0 - Nothing                                                        \
   * 1 - Warn                                                           \
   * 2 - Throw exception                                                \
   */                                                                   \
  F(uint32_t, EnforceModules, 0)                                        \
  /*                                                                    \
   * Controls behavior on reflection to default value expressions       \
   * that throw during evaluation                                       \
   * 0 - Nothing                                                        \
   * 1 - Warn and retain current behavior                               \
   * 2 - Return null for parameter value                                \
   */                                                                   \
  F(int32_t, FixDefaultArgReflection, 2)                                \
  F(int32_t, ServerOOMAdj, 0)                                           \
  F(std::string, PreludePath, "")                                       \
  F(uint32_t, NonSharedInstanceMemoCaches, 10)                          \
  F(bool, UseGraphColor, true)                                          \
  F(std::vector<std::string>, IniGetHide, std::vector<std::string>())   \
  F(std::string, UseRemoteUnixServer, "no")                             \
  F(std::string, UnixServerPath, "")                                    \
  F(uint32_t, UnixServerWorkers, Process::GetCPUCount())                \
  F(bool, UnixServerFailWhenBusy, false)                                \
  F(std::vector<std::string>, UnixServerAllowedUsers,                   \
                                            std::vector<std::string>()) \
  F(std::vector<std::string>, UnixServerAllowedGroups,                  \
                                            std::vector<std::string>()) \
  F(bool, UnixServerRunPSPInBackground, true)                           \
  F(bool, UnixServerProxyXbox, true)                                    \
  F(bool, UnixServerAssumeRepoReadable, true)                           \
  F(bool, UnixServerAssumeRepoRealpath, true)                           \
  /* Options for testing */                                             \
  F(bool, TrashFillOnRequestExit, false)                                \
  /********************                                                 \
   | Profiling flags. |                                                 \
   ********************/                                                \
  /* Whether to maintain the address-to-VM-object mapping. */           \
  F(bool, EnableReverseDataMap, true)                                   \
  /* Turn on perf-mem-event sampling roughly every this many requests.  \
   * To maintain the same overall sampling rate, the ratio between the  \
   * request and sample frequencies should be kept constant. */         \
  F(uint32_t, PerfMemEventRequestFreq, 0)                               \
  /* Sample this many memory instructions per second.  This should be   \
   * kept low to avoid the risk of collecting a sample while we're      \
   * processing a previous sample. */                                   \
  F(uint32_t, PerfMemEventSampleFreq, 80)                               \
  /* Sampling frequency for TC branch profiling. */                     \
  F(uint32_t, ProfBranchSampleFreq, 0)                                  \
  /* Record the first N units loaded via StructuredLog::log()        */ \
  F(uint64_t, RecordFirstUnits, 0)                                      \
  /* More aggressively reuse already compiled units based on SHA1    */ \
  F(bool, CheckUnitSHA1, true)                                          \
  F(bool, ReuseUnitsByHash, false)                                      \
  F(bool, UseEdenFS, true)                                              \
  /* Arbitrary string to force different unit-cache hashes */           \
  F(std::string, UnitCacheBreaker, "")                                  \
  /* When dynamic_fun is called on a function not marked as
     __DynamicallyCallable:

     0 - do nothing
     1 - raise a warning
     2 - throw */                                                       \
  F(uint64_t, DynamicFunLevel, 1)                                       \
  /* When dynamic_class_meth is called on a method not marked as
     __DynamicallyCallable:

     0 - do nothing
     1 - raise a warning
     2 - throw */                                                       \
  F(uint64_t, DynamicClsMethLevel, 1)                                   \
  /* When dynamic_meth_caller is called on a static method or
     a method not marked as __DynamicallyCallable:

     0 - do nothing
     1 - raise a warning
     2 - throw */                                                       \
  F(uint64_t, DynamicMethCallerLevel, 1)                                \
  F(bool, APCSerializeFuncs, true)                                      \
  F(bool, APCSerializeClsMeth, true)                                    \
  F(bool, LogOnIsArrayFunction, false)                                  \
  /* Unit prefetching options */                                        \
  F(uint32_t, UnitPrefetcherMaxThreads, 0)                              \
  F(uint32_t, UnitPrefetcherMinThreads, 0)                              \
  F(uint32_t, UnitPrefetcherIdleThreadTimeoutSecs, 60)                  \
  /* Delete any Unit not used in last N seconds */                      \
  F(uint32_t, IdleUnitTimeoutSecs, 0)                                   \
  /* Don't reap total Units below threshold */                          \
  F(uint32_t, IdleUnitMinThreshold, 0)                                  \
  /* 0 nothing, 1 notice, 2 error */                                    \
  F(int32_t, NoticeOnCoerceForStrConcat, 0)                             \
  /* 0 nothing, 1 notice, 2 error */                                    \
  F(int32_t, NoticeOnCoerceForStrConcat2, 0)                            \
  F(string, TaoMigrationOverride, std::string(""))                      \
  F(string, SRRouteMigrationOverride, std::string(""))                  \
  F(int32_t, SampleRequestTearing, 0)                                   \
  F(int32_t, RequestTearingSkewMicros, 1500)                            \
  F(bool,    SampleRequestTearingForce, true)                           \
  F(bool, EnableAbstractContextConstants, true)                         \
  /* The maximum number of resolved variants allowed in a single case
     type. This value is determined after flattening. */                \
  F(uint32_t, MaxCaseTypeVariants, 48)                                  \
  F(uint32_t, LogSlowWatchmanQueriesMsec, 500)                          \
  F(uint32_t, LogSlowWatchmanQueriesRate, 1)                            \
  F(uint32_t, StartOptionLogRate, 0)                                    \
  F(std::string, StartOptionLogCache, "/tmp/hhvm-options-%{user}-%{hash}")\
  F(uint64_t, StartOptionLogWindow, 86400)                              \
  F(hphp_fast_string_set, StartOptionLogOptions, {})                    \
  F(hphp_fast_string_set, StartOptionLogExcludeOptions, {})             \
  F(bool, RecordReplay, false)                                          \
  /* Format: _SUPERGLOBAL.Key=Value[&_SUPERGLOBAL.Key=Value...] */      \
  /* Example: _SERVER.SCRIPT_URL=/foo&_POST.key=value */                \
  F(string, RecordSampleFilter, std::string(""))                        \
  F(uint64_t, RecordSampleRate, 0)                                      \
  F(string, RecordDir, std::string(""))                                 \
  F(bool, Replay, false)                                                \
  F(bool, DumpStacktraceToErrorLogOnCrash, true)                        \
  F(bool, IncludeReopOptionsInFactsCacheBreaker, false)                 \
  F(bool, AutoloadEagerSyncUnitCache, true)                             \
  F(bool, AutoloadEagerReloadUnitCache, true)                           \
  F(bool, AutoloadInitEarly, false)                                     \
  /* Whether we should dump the request headers into $_SERVER */        \
  F(bool, SetHeadersInServerSuperGlobal, true)                          \
  /* Whether we should stop parsing cookies out of the headers and
     setting it into a few super globals - including fully removing
     the existance of the $_COOKIE superglobal */                       \
  F(bool, DisableParsedCookies, false)                                  \
  /* Whether to remove the existence of the REQUEST superglobal */      \
  F(bool, DisableRequestSuperglobal, false)                             \
  /* Enables the non-surprise flag based implementation of
     fb_intercept2 */                                                   \
  F(bool, FastMethodIntercept, false)                                   \
  F(bool, LogHttpServerSignalSource, true)                              \
  F(bool, CrashOnStaticAnalysisError, debug)                            \
  /* */

private:
  using string = std::string;

  // Custom settings. This should be accessed via the GetServerCustomSetting
  // APIs.
  static std::map<std::string, std::string> CustomSettings;

public:
#define F(type, name, unused) \
  static type Eval ## name;
  EVALFLAGS()
#undef F

  static bool RecordCodeCoverage;
  static std::string CodeCoverageOutputFile;

  // Repo (hhvm bytecode repository) options
  static std::string RepoPath;
  static bool RepoLitstrLazyLoad;
  static bool RepoDebugInfo;
  static bool RepoAuthoritative;

  // These are (functionally) unused
  static RepoMode RepoLocalMode;
  static std::string RepoLocalPath;
  static RepoMode RepoCentralMode;
  static std::string RepoCentralPath;
  static int32_t RepoCentralFileMode;
  static std::string RepoCentralFileUser;
  static std::string RepoCentralFileGroup;
  static bool RepoAllowFallbackPath;
  static std::string RepoJournal;
  static bool RepoCommit;
  static uint32_t RepoBusyTimeoutMS;

  // pprof/hhprof options
  static bool HHProfEnabled;
  static bool HHProfActive;
  static bool HHProfAccum;
  static bool HHProfRequest;
  static bool TrackPerUnitMemory;

  // Sandbox options
  static bool SandboxMode;
  static std::string SandboxPattern;
  static std::string SandboxHome;
  static std::string SandboxFallback;
  static std::string SandboxConfFile;
  static std::map<std::string, std::string> SandboxServerVariables;
  static bool SandboxFromCommonRoot;
  static std::string SandboxDirectoriesRoot;
  static std::string SandboxLogsRoot;
  static std::string SandboxDefaultUserFile;
  static std::string SandboxHostAlias;

  // Mail options
  static std::string SendmailPath;
  static std::string MailForceExtraParameters;

  // SimpleXML options
  static bool SimpleXMLEmptyNamespaceMatchesAll;

#ifdef HHVM_FACEBOOK
  // ThriftFBServer
  static int ThriftFBServerWorkerThreads;
  static int ThriftFBServerPoolThreads;

  // fb303 server
  static bool EnableFb303Server;
  static int Fb303ServerPort;
  static std::string Fb303ServerIP;
  static int Fb303ServerWorkerThreads;
  static int Fb303ServerPoolThreads;
  static bool Fb303ServerExposeSensitiveMethods;

  // Experimental thread tuning options, allows threads to be adjusted by
  // thread controller (host stats monitor). `ThreadTuneDebug` is meant to allow
  // additional debugging metrics/logs to be exported. `ThreadTuneSkipWarmup`
  // will skip the warmup period (jit maturity = 100). Maximum adjustment is
  // defined by the `ThreadTuneAdjustmentPct` of the configured thread count,
  // and the step size is defined by `ThreadTuneStepPct`. Thread tuning is
  // turned off when `ThreadTuneAdjustmentPct` is set to 0 (default).
  static bool ThreadTuneDebug;
  static bool ThreadTuneSkipWarmup;
  static double ThreadTuneAdjustmentPct;
  static double ThreadTuneAdjustmentDownPct;
  static double ThreadTuneStepPct;
  // CPU high threshold is used for determining when to adjust threads. If the
  // host CPU is > this threshold no adjustments will be made.
  static double ThreadTuneCPUThreshold;
  // Thread utilization threshold is used for determining when to adjust threads,
  // threads will be increased if other criteria match and the current thread
  // utilization is above this threshold.
  static double ThreadTuneThreadUtilizationThreshold;
#endif

  // Xenon options
  static double XenonPeriodSeconds;
  static uint32_t XenonRequestFreq;
  static bool XenonForceAlwaysOn;
  static bool XenonTrackActiveWorkers;

  // Strobelight options
  static bool StrobelightEnabled;

  static bool SetProfileNullThisObject;

  static bool funcIsRenamable(const StringData* name);
};
static_assert(sizeof(RuntimeOption) == 1, "no instance variables");

using RO = RuntimeOption;

inline bool isJitDeserializing() {
  auto const m = RuntimeOption::EvalJitSerdesMode;
  return static_cast<std::underlying_type<JitSerdesMode>::type>(m) & 0x2;
}

inline bool isJitSerializing() {
  auto const m = RuntimeOption::EvalJitSerdesMode;
  return static_cast<std::underlying_type<JitSerdesMode>::type>(m) & 0x1;
}

inline bool unitPrefetchingEnabled() {
  return RO::EvalUnitPrefetcherMaxThreads > 0;
}

inline StringToIntMap coeffectEnforcementLevelsDefaults() {
#ifdef HHVM_FACEBOOK
  return {{"zoned", 2}};
#else
  return {};
#endif
}

uintptr_t lowArenaMinAddr();

///////////////////////////////////////////////////////////////////////////////
}
