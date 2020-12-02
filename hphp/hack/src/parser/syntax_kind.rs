/**
 * Copyright (c) 2016, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree. An additional
 * directory.
 *
 **
 *
 * THIS FILE IS @generated; DO NOT EDIT IT
 * To regenerate this file, run
 *
 *   buck run //hphp/hack/src:generate_full_fidelity
 *
 **
 *
 */

use ocamlrep_derive::{FromOcamlRep, ToOcamlRep};

use crate::token_kind::TokenKind;

#[derive(Debug, Copy, Clone, FromOcamlRep, ToOcamlRep, PartialEq)]
pub enum SyntaxKind {
    Missing,
    Token(TokenKind),
    SyntaxList,
    EndOfFile,
    Script,
    QualifiedName,
    SimpleTypeSpecifier,
    LiteralExpression,
    PrefixedStringExpression,
    PrefixedCodeExpression,
    VariableExpression,
    PipeVariableExpression,
    FileAttributeSpecification,
    EnumDeclaration,
    Enumerator,
    EnumClassDeclaration,
    EnumClassEnumerator,
    RecordDeclaration,
    RecordField,
    AliasDeclaration,
    PropertyDeclaration,
    PropertyDeclarator,
    NamespaceDeclaration,
    NamespaceDeclarationHeader,
    NamespaceBody,
    NamespaceEmptyBody,
    NamespaceUseDeclaration,
    NamespaceGroupUseDeclaration,
    NamespaceUseClause,
    FunctionDeclaration,
    FunctionDeclarationHeader,
    Capability,
    CapabilityProvisional,
    WhereClause,
    WhereConstraint,
    MethodishDeclaration,
    MethodishTraitResolution,
    ClassishDeclaration,
    ClassishBody,
    TraitUsePrecedenceItem,
    TraitUseAliasItem,
    TraitUseConflictResolution,
    TraitUse,
    RequireClause,
    ConstDeclaration,
    ConstantDeclarator,
    TypeConstDeclaration,
    DecoratedExpression,
    ParameterDeclaration,
    VariadicParameter,
    OldAttributeSpecification,
    AttributeSpecification,
    Attribute,
    InclusionExpression,
    InclusionDirective,
    CompoundStatement,
    ExpressionStatement,
    MarkupSection,
    MarkupSuffix,
    UnsetStatement,
    UsingStatementBlockScoped,
    UsingStatementFunctionScoped,
    WhileStatement,
    IfStatement,
    ElseifClause,
    ElseClause,
    TryStatement,
    CatchClause,
    FinallyClause,
    DoStatement,
    ForStatement,
    ForeachStatement,
    SwitchStatement,
    SwitchSection,
    SwitchFallthrough,
    CaseLabel,
    DefaultLabel,
    ReturnStatement,
    ThrowStatement,
    BreakStatement,
    ContinueStatement,
    EchoStatement,
    ConcurrentStatement,
    SimpleInitializer,
    AnonymousClass,
    AnonymousFunction,
    AnonymousFunctionUseClause,
    LambdaExpression,
    LambdaSignature,
    CastExpression,
    ScopeResolutionExpression,
    MemberSelectionExpression,
    SafeMemberSelectionExpression,
    EmbeddedMemberSelectionExpression,
    YieldExpression,
    PrefixUnaryExpression,
    PostfixUnaryExpression,
    BinaryExpression,
    IsExpression,
    AsExpression,
    NullableAsExpression,
    ConditionalExpression,
    EvalExpression,
    DefineExpression,
    IssetExpression,
    FunctionCallExpression,
    FunctionPointerExpression,
    ParenthesizedExpression,
    BracedExpression,
    EmbeddedBracedExpression,
    ListExpression,
    CollectionLiteralExpression,
    ObjectCreationExpression,
    ConstructorCall,
    RecordCreationExpression,
    DarrayIntrinsicExpression,
    DictionaryIntrinsicExpression,
    KeysetIntrinsicExpression,
    VarrayIntrinsicExpression,
    VectorIntrinsicExpression,
    ElementInitializer,
    SubscriptExpression,
    EmbeddedSubscriptExpression,
    AwaitableCreationExpression,
    XHPChildrenDeclaration,
    XHPChildrenParenthesizedList,
    XHPCategoryDeclaration,
    XHPEnumType,
    XHPLateinit,
    XHPRequired,
    XHPClassAttributeDeclaration,
    XHPClassAttribute,
    XHPSimpleClassAttribute,
    XHPSimpleAttribute,
    XHPSpreadAttribute,
    XHPOpen,
    XHPExpression,
    XHPClose,
    TypeConstant,
    VectorTypeSpecifier,
    KeysetTypeSpecifier,
    TupleTypeExplicitSpecifier,
    VarrayTypeSpecifier,
    TypeParameter,
    TypeConstraint,
    DarrayTypeSpecifier,
    DictionaryTypeSpecifier,
    ClosureTypeSpecifier,
    ClosureParameterTypeSpecifier,
    ClassnameTypeSpecifier,
    FieldSpecifier,
    FieldInitializer,
    ShapeTypeSpecifier,
    ShapeExpression,
    TupleExpression,
    GenericTypeSpecifier,
    NullableTypeSpecifier,
    LikeTypeSpecifier,
    SoftTypeSpecifier,
    AttributizedSpecifier,
    ReifiedTypeArgument,
    TypeArguments,
    TypeParameters,
    TupleTypeSpecifier,
    UnionTypeSpecifier,
    IntersectionTypeSpecifier,
    ErrorSyntax,
    ListItem,
    EnumAtomExpression,

}

impl SyntaxKind {
    pub fn to_string(&self) -> &str {
        match self {
            SyntaxKind::SyntaxList => "list",
            SyntaxKind::Missing => "missing",
            SyntaxKind::Token(_) => "token",
            SyntaxKind::EndOfFile                         => "end_of_file",
            SyntaxKind::Script                            => "script",
            SyntaxKind::QualifiedName                     => "qualified_name",
            SyntaxKind::SimpleTypeSpecifier               => "simple_type_specifier",
            SyntaxKind::LiteralExpression                 => "literal",
            SyntaxKind::PrefixedStringExpression          => "prefixed_string",
            SyntaxKind::PrefixedCodeExpression            => "prefixed_code",
            SyntaxKind::VariableExpression                => "variable",
            SyntaxKind::PipeVariableExpression            => "pipe_variable",
            SyntaxKind::FileAttributeSpecification        => "file_attribute_specification",
            SyntaxKind::EnumDeclaration                   => "enum_declaration",
            SyntaxKind::Enumerator                        => "enumerator",
            SyntaxKind::EnumClassDeclaration              => "enum_class_declaration",
            SyntaxKind::EnumClassEnumerator               => "enum_class_enumerator",
            SyntaxKind::RecordDeclaration                 => "record_declaration",
            SyntaxKind::RecordField                       => "record_field",
            SyntaxKind::AliasDeclaration                  => "alias_declaration",
            SyntaxKind::PropertyDeclaration               => "property_declaration",
            SyntaxKind::PropertyDeclarator                => "property_declarator",
            SyntaxKind::NamespaceDeclaration              => "namespace_declaration",
            SyntaxKind::NamespaceDeclarationHeader        => "namespace_declaration_header",
            SyntaxKind::NamespaceBody                     => "namespace_body",
            SyntaxKind::NamespaceEmptyBody                => "namespace_empty_body",
            SyntaxKind::NamespaceUseDeclaration           => "namespace_use_declaration",
            SyntaxKind::NamespaceGroupUseDeclaration      => "namespace_group_use_declaration",
            SyntaxKind::NamespaceUseClause                => "namespace_use_clause",
            SyntaxKind::FunctionDeclaration               => "function_declaration",
            SyntaxKind::FunctionDeclarationHeader         => "function_declaration_header",
            SyntaxKind::Capability                        => "capability",
            SyntaxKind::CapabilityProvisional             => "capability_provisional",
            SyntaxKind::WhereClause                       => "where_clause",
            SyntaxKind::WhereConstraint                   => "where_constraint",
            SyntaxKind::MethodishDeclaration              => "methodish_declaration",
            SyntaxKind::MethodishTraitResolution          => "methodish_trait_resolution",
            SyntaxKind::ClassishDeclaration               => "classish_declaration",
            SyntaxKind::ClassishBody                      => "classish_body",
            SyntaxKind::TraitUsePrecedenceItem            => "trait_use_precedence_item",
            SyntaxKind::TraitUseAliasItem                 => "trait_use_alias_item",
            SyntaxKind::TraitUseConflictResolution        => "trait_use_conflict_resolution",
            SyntaxKind::TraitUse                          => "trait_use",
            SyntaxKind::RequireClause                     => "require_clause",
            SyntaxKind::ConstDeclaration                  => "const_declaration",
            SyntaxKind::ConstantDeclarator                => "constant_declarator",
            SyntaxKind::TypeConstDeclaration              => "type_const_declaration",
            SyntaxKind::DecoratedExpression               => "decorated_expression",
            SyntaxKind::ParameterDeclaration              => "parameter_declaration",
            SyntaxKind::VariadicParameter                 => "variadic_parameter",
            SyntaxKind::OldAttributeSpecification         => "old_attribute_specification",
            SyntaxKind::AttributeSpecification            => "attribute_specification",
            SyntaxKind::Attribute                         => "attribute",
            SyntaxKind::InclusionExpression               => "inclusion_expression",
            SyntaxKind::InclusionDirective                => "inclusion_directive",
            SyntaxKind::CompoundStatement                 => "compound_statement",
            SyntaxKind::ExpressionStatement               => "expression_statement",
            SyntaxKind::MarkupSection                     => "markup_section",
            SyntaxKind::MarkupSuffix                      => "markup_suffix",
            SyntaxKind::UnsetStatement                    => "unset_statement",
            SyntaxKind::UsingStatementBlockScoped         => "using_statement_block_scoped",
            SyntaxKind::UsingStatementFunctionScoped      => "using_statement_function_scoped",
            SyntaxKind::WhileStatement                    => "while_statement",
            SyntaxKind::IfStatement                       => "if_statement",
            SyntaxKind::ElseifClause                      => "elseif_clause",
            SyntaxKind::ElseClause                        => "else_clause",
            SyntaxKind::TryStatement                      => "try_statement",
            SyntaxKind::CatchClause                       => "catch_clause",
            SyntaxKind::FinallyClause                     => "finally_clause",
            SyntaxKind::DoStatement                       => "do_statement",
            SyntaxKind::ForStatement                      => "for_statement",
            SyntaxKind::ForeachStatement                  => "foreach_statement",
            SyntaxKind::SwitchStatement                   => "switch_statement",
            SyntaxKind::SwitchSection                     => "switch_section",
            SyntaxKind::SwitchFallthrough                 => "switch_fallthrough",
            SyntaxKind::CaseLabel                         => "case_label",
            SyntaxKind::DefaultLabel                      => "default_label",
            SyntaxKind::ReturnStatement                   => "return_statement",
            SyntaxKind::ThrowStatement                    => "throw_statement",
            SyntaxKind::BreakStatement                    => "break_statement",
            SyntaxKind::ContinueStatement                 => "continue_statement",
            SyntaxKind::EchoStatement                     => "echo_statement",
            SyntaxKind::ConcurrentStatement               => "concurrent_statement",
            SyntaxKind::SimpleInitializer                 => "simple_initializer",
            SyntaxKind::AnonymousClass                    => "anonymous_class",
            SyntaxKind::AnonymousFunction                 => "anonymous_function",
            SyntaxKind::AnonymousFunctionUseClause        => "anonymous_function_use_clause",
            SyntaxKind::LambdaExpression                  => "lambda_expression",
            SyntaxKind::LambdaSignature                   => "lambda_signature",
            SyntaxKind::CastExpression                    => "cast_expression",
            SyntaxKind::ScopeResolutionExpression         => "scope_resolution_expression",
            SyntaxKind::MemberSelectionExpression         => "member_selection_expression",
            SyntaxKind::SafeMemberSelectionExpression     => "safe_member_selection_expression",
            SyntaxKind::EmbeddedMemberSelectionExpression => "embedded_member_selection_expression",
            SyntaxKind::YieldExpression                   => "yield_expression",
            SyntaxKind::PrefixUnaryExpression             => "prefix_unary_expression",
            SyntaxKind::PostfixUnaryExpression            => "postfix_unary_expression",
            SyntaxKind::BinaryExpression                  => "binary_expression",
            SyntaxKind::IsExpression                      => "is_expression",
            SyntaxKind::AsExpression                      => "as_expression",
            SyntaxKind::NullableAsExpression              => "nullable_as_expression",
            SyntaxKind::ConditionalExpression             => "conditional_expression",
            SyntaxKind::EvalExpression                    => "eval_expression",
            SyntaxKind::DefineExpression                  => "define_expression",
            SyntaxKind::IssetExpression                   => "isset_expression",
            SyntaxKind::FunctionCallExpression            => "function_call_expression",
            SyntaxKind::FunctionPointerExpression         => "function_pointer_expression",
            SyntaxKind::ParenthesizedExpression           => "parenthesized_expression",
            SyntaxKind::BracedExpression                  => "braced_expression",
            SyntaxKind::EmbeddedBracedExpression          => "embedded_braced_expression",
            SyntaxKind::ListExpression                    => "list_expression",
            SyntaxKind::CollectionLiteralExpression       => "collection_literal_expression",
            SyntaxKind::ObjectCreationExpression          => "object_creation_expression",
            SyntaxKind::ConstructorCall                   => "constructor_call",
            SyntaxKind::RecordCreationExpression          => "record_creation_expression",
            SyntaxKind::DarrayIntrinsicExpression         => "darray_intrinsic_expression",
            SyntaxKind::DictionaryIntrinsicExpression     => "dictionary_intrinsic_expression",
            SyntaxKind::KeysetIntrinsicExpression         => "keyset_intrinsic_expression",
            SyntaxKind::VarrayIntrinsicExpression         => "varray_intrinsic_expression",
            SyntaxKind::VectorIntrinsicExpression         => "vector_intrinsic_expression",
            SyntaxKind::ElementInitializer                => "element_initializer",
            SyntaxKind::SubscriptExpression               => "subscript_expression",
            SyntaxKind::EmbeddedSubscriptExpression       => "embedded_subscript_expression",
            SyntaxKind::AwaitableCreationExpression       => "awaitable_creation_expression",
            SyntaxKind::XHPChildrenDeclaration            => "xhp_children_declaration",
            SyntaxKind::XHPChildrenParenthesizedList      => "xhp_children_parenthesized_list",
            SyntaxKind::XHPCategoryDeclaration            => "xhp_category_declaration",
            SyntaxKind::XHPEnumType                       => "xhp_enum_type",
            SyntaxKind::XHPLateinit                       => "xhp_lateinit",
            SyntaxKind::XHPRequired                       => "xhp_required",
            SyntaxKind::XHPClassAttributeDeclaration      => "xhp_class_attribute_declaration",
            SyntaxKind::XHPClassAttribute                 => "xhp_class_attribute",
            SyntaxKind::XHPSimpleClassAttribute           => "xhp_simple_class_attribute",
            SyntaxKind::XHPSimpleAttribute                => "xhp_simple_attribute",
            SyntaxKind::XHPSpreadAttribute                => "xhp_spread_attribute",
            SyntaxKind::XHPOpen                           => "xhp_open",
            SyntaxKind::XHPExpression                     => "xhp_expression",
            SyntaxKind::XHPClose                          => "xhp_close",
            SyntaxKind::TypeConstant                      => "type_constant",
            SyntaxKind::VectorTypeSpecifier               => "vector_type_specifier",
            SyntaxKind::KeysetTypeSpecifier               => "keyset_type_specifier",
            SyntaxKind::TupleTypeExplicitSpecifier        => "tuple_type_explicit_specifier",
            SyntaxKind::VarrayTypeSpecifier               => "varray_type_specifier",
            SyntaxKind::TypeParameter                     => "type_parameter",
            SyntaxKind::TypeConstraint                    => "type_constraint",
            SyntaxKind::DarrayTypeSpecifier               => "darray_type_specifier",
            SyntaxKind::DictionaryTypeSpecifier           => "dictionary_type_specifier",
            SyntaxKind::ClosureTypeSpecifier              => "closure_type_specifier",
            SyntaxKind::ClosureParameterTypeSpecifier     => "closure_parameter_type_specifier",
            SyntaxKind::ClassnameTypeSpecifier            => "classname_type_specifier",
            SyntaxKind::FieldSpecifier                    => "field_specifier",
            SyntaxKind::FieldInitializer                  => "field_initializer",
            SyntaxKind::ShapeTypeSpecifier                => "shape_type_specifier",
            SyntaxKind::ShapeExpression                   => "shape_expression",
            SyntaxKind::TupleExpression                   => "tuple_expression",
            SyntaxKind::GenericTypeSpecifier              => "generic_type_specifier",
            SyntaxKind::NullableTypeSpecifier             => "nullable_type_specifier",
            SyntaxKind::LikeTypeSpecifier                 => "like_type_specifier",
            SyntaxKind::SoftTypeSpecifier                 => "soft_type_specifier",
            SyntaxKind::AttributizedSpecifier             => "attributized_specifier",
            SyntaxKind::ReifiedTypeArgument               => "reified_type_argument",
            SyntaxKind::TypeArguments                     => "type_arguments",
            SyntaxKind::TypeParameters                    => "type_parameters",
            SyntaxKind::TupleTypeSpecifier                => "tuple_type_specifier",
            SyntaxKind::UnionTypeSpecifier                => "union_type_specifier",
            SyntaxKind::IntersectionTypeSpecifier         => "intersection_type_specifier",
            SyntaxKind::ErrorSyntax                       => "error",
            SyntaxKind::ListItem                          => "list_item",
            SyntaxKind::EnumAtomExpression                => "enum_atom",
        }
    }

    pub fn ocaml_tag(self) -> u8 {
        match self {
            SyntaxKind::Missing => 0,
            SyntaxKind::Token(_) => 0,
            SyntaxKind::SyntaxList => 1,
            SyntaxKind::EndOfFile => 2,
            SyntaxKind::Script => 3,
            SyntaxKind::QualifiedName => 4,
            SyntaxKind::SimpleTypeSpecifier => 5,
            SyntaxKind::LiteralExpression => 6,
            SyntaxKind::PrefixedStringExpression => 7,
            SyntaxKind::PrefixedCodeExpression => 8,
            SyntaxKind::VariableExpression => 9,
            SyntaxKind::PipeVariableExpression => 10,
            SyntaxKind::FileAttributeSpecification => 11,
            SyntaxKind::EnumDeclaration => 12,
            SyntaxKind::Enumerator => 13,
            SyntaxKind::EnumClassDeclaration => 14,
            SyntaxKind::EnumClassEnumerator => 15,
            SyntaxKind::RecordDeclaration => 16,
            SyntaxKind::RecordField => 17,
            SyntaxKind::AliasDeclaration => 18,
            SyntaxKind::PropertyDeclaration => 19,
            SyntaxKind::PropertyDeclarator => 20,
            SyntaxKind::NamespaceDeclaration => 21,
            SyntaxKind::NamespaceDeclarationHeader => 22,
            SyntaxKind::NamespaceBody => 23,
            SyntaxKind::NamespaceEmptyBody => 24,
            SyntaxKind::NamespaceUseDeclaration => 25,
            SyntaxKind::NamespaceGroupUseDeclaration => 26,
            SyntaxKind::NamespaceUseClause => 27,
            SyntaxKind::FunctionDeclaration => 28,
            SyntaxKind::FunctionDeclarationHeader => 29,
            SyntaxKind::Capability => 30,
            SyntaxKind::CapabilityProvisional => 31,
            SyntaxKind::WhereClause => 32,
            SyntaxKind::WhereConstraint => 33,
            SyntaxKind::MethodishDeclaration => 34,
            SyntaxKind::MethodishTraitResolution => 35,
            SyntaxKind::ClassishDeclaration => 36,
            SyntaxKind::ClassishBody => 37,
            SyntaxKind::TraitUsePrecedenceItem => 38,
            SyntaxKind::TraitUseAliasItem => 39,
            SyntaxKind::TraitUseConflictResolution => 40,
            SyntaxKind::TraitUse => 41,
            SyntaxKind::RequireClause => 42,
            SyntaxKind::ConstDeclaration => 43,
            SyntaxKind::ConstantDeclarator => 44,
            SyntaxKind::TypeConstDeclaration => 45,
            SyntaxKind::DecoratedExpression => 46,
            SyntaxKind::ParameterDeclaration => 47,
            SyntaxKind::VariadicParameter => 48,
            SyntaxKind::OldAttributeSpecification => 49,
            SyntaxKind::AttributeSpecification => 50,
            SyntaxKind::Attribute => 51,
            SyntaxKind::InclusionExpression => 52,
            SyntaxKind::InclusionDirective => 53,
            SyntaxKind::CompoundStatement => 54,
            SyntaxKind::ExpressionStatement => 55,
            SyntaxKind::MarkupSection => 56,
            SyntaxKind::MarkupSuffix => 57,
            SyntaxKind::UnsetStatement => 58,
            SyntaxKind::UsingStatementBlockScoped => 59,
            SyntaxKind::UsingStatementFunctionScoped => 60,
            SyntaxKind::WhileStatement => 61,
            SyntaxKind::IfStatement => 62,
            SyntaxKind::ElseifClause => 63,
            SyntaxKind::ElseClause => 64,
            SyntaxKind::TryStatement => 65,
            SyntaxKind::CatchClause => 66,
            SyntaxKind::FinallyClause => 67,
            SyntaxKind::DoStatement => 68,
            SyntaxKind::ForStatement => 69,
            SyntaxKind::ForeachStatement => 70,
            SyntaxKind::SwitchStatement => 71,
            SyntaxKind::SwitchSection => 72,
            SyntaxKind::SwitchFallthrough => 73,
            SyntaxKind::CaseLabel => 74,
            SyntaxKind::DefaultLabel => 75,
            SyntaxKind::ReturnStatement => 76,
            SyntaxKind::ThrowStatement => 77,
            SyntaxKind::BreakStatement => 78,
            SyntaxKind::ContinueStatement => 79,
            SyntaxKind::EchoStatement => 80,
            SyntaxKind::ConcurrentStatement => 81,
            SyntaxKind::SimpleInitializer => 82,
            SyntaxKind::AnonymousClass => 83,
            SyntaxKind::AnonymousFunction => 84,
            SyntaxKind::AnonymousFunctionUseClause => 85,
            SyntaxKind::LambdaExpression => 86,
            SyntaxKind::LambdaSignature => 87,
            SyntaxKind::CastExpression => 88,
            SyntaxKind::ScopeResolutionExpression => 89,
            SyntaxKind::MemberSelectionExpression => 90,
            SyntaxKind::SafeMemberSelectionExpression => 91,
            SyntaxKind::EmbeddedMemberSelectionExpression => 92,
            SyntaxKind::YieldExpression => 93,
            SyntaxKind::PrefixUnaryExpression => 94,
            SyntaxKind::PostfixUnaryExpression => 95,
            SyntaxKind::BinaryExpression => 96,
            SyntaxKind::IsExpression => 97,
            SyntaxKind::AsExpression => 98,
            SyntaxKind::NullableAsExpression => 99,
            SyntaxKind::ConditionalExpression => 100,
            SyntaxKind::EvalExpression => 101,
            SyntaxKind::DefineExpression => 102,
            SyntaxKind::IssetExpression => 103,
            SyntaxKind::FunctionCallExpression => 104,
            SyntaxKind::FunctionPointerExpression => 105,
            SyntaxKind::ParenthesizedExpression => 106,
            SyntaxKind::BracedExpression => 107,
            SyntaxKind::EmbeddedBracedExpression => 108,
            SyntaxKind::ListExpression => 109,
            SyntaxKind::CollectionLiteralExpression => 110,
            SyntaxKind::ObjectCreationExpression => 111,
            SyntaxKind::ConstructorCall => 112,
            SyntaxKind::RecordCreationExpression => 113,
            SyntaxKind::DarrayIntrinsicExpression => 114,
            SyntaxKind::DictionaryIntrinsicExpression => 115,
            SyntaxKind::KeysetIntrinsicExpression => 116,
            SyntaxKind::VarrayIntrinsicExpression => 117,
            SyntaxKind::VectorIntrinsicExpression => 118,
            SyntaxKind::ElementInitializer => 119,
            SyntaxKind::SubscriptExpression => 120,
            SyntaxKind::EmbeddedSubscriptExpression => 121,
            SyntaxKind::AwaitableCreationExpression => 122,
            SyntaxKind::XHPChildrenDeclaration => 123,
            SyntaxKind::XHPChildrenParenthesizedList => 124,
            SyntaxKind::XHPCategoryDeclaration => 125,
            SyntaxKind::XHPEnumType => 126,
            SyntaxKind::XHPLateinit => 127,
            SyntaxKind::XHPRequired => 128,
            SyntaxKind::XHPClassAttributeDeclaration => 129,
            SyntaxKind::XHPClassAttribute => 130,
            SyntaxKind::XHPSimpleClassAttribute => 131,
            SyntaxKind::XHPSimpleAttribute => 132,
            SyntaxKind::XHPSpreadAttribute => 133,
            SyntaxKind::XHPOpen => 134,
            SyntaxKind::XHPExpression => 135,
            SyntaxKind::XHPClose => 136,
            SyntaxKind::TypeConstant => 137,
            SyntaxKind::VectorTypeSpecifier => 138,
            SyntaxKind::KeysetTypeSpecifier => 139,
            SyntaxKind::TupleTypeExplicitSpecifier => 140,
            SyntaxKind::VarrayTypeSpecifier => 141,
            SyntaxKind::TypeParameter => 142,
            SyntaxKind::TypeConstraint => 143,
            SyntaxKind::DarrayTypeSpecifier => 144,
            SyntaxKind::DictionaryTypeSpecifier => 145,
            SyntaxKind::ClosureTypeSpecifier => 146,
            SyntaxKind::ClosureParameterTypeSpecifier => 147,
            SyntaxKind::ClassnameTypeSpecifier => 148,
            SyntaxKind::FieldSpecifier => 149,
            SyntaxKind::FieldInitializer => 150,
            SyntaxKind::ShapeTypeSpecifier => 151,
            SyntaxKind::ShapeExpression => 152,
            SyntaxKind::TupleExpression => 153,
            SyntaxKind::GenericTypeSpecifier => 154,
            SyntaxKind::NullableTypeSpecifier => 155,
            SyntaxKind::LikeTypeSpecifier => 156,
            SyntaxKind::SoftTypeSpecifier => 157,
            SyntaxKind::AttributizedSpecifier => 158,
            SyntaxKind::ReifiedTypeArgument => 159,
            SyntaxKind::TypeArguments => 160,
            SyntaxKind::TypeParameters => 161,
            SyntaxKind::TupleTypeSpecifier => 162,
            SyntaxKind::UnionTypeSpecifier => 163,
            SyntaxKind::IntersectionTypeSpecifier => 164,
            SyntaxKind::ErrorSyntax => 165,
            SyntaxKind::ListItem => 166,
            SyntaxKind::EnumAtomExpression => 167,
        }
    }
}
