// @generated by Thrift for thrift/compiler/test/fixtures/exceptions/src/module.thrift
// This file is probably not the place you want to edit!

//! Thrift service definitions for `module`.


/// Service definitions for `Raiser`.
pub mod raiser {
    #[derive(Clone, Debug)]
    pub enum DoBlandExn {

        ApplicationException(::fbthrift::ApplicationException),
    }

    impl ::std::convert::From<DoBlandExn> for ::fbthrift::NonthrowingFunctionError {
        fn from(err: DoBlandExn) -> Self {
            match err {
                DoBlandExn::ApplicationException(aexn) => ::fbthrift::NonthrowingFunctionError::ApplicationException(aexn),
            }
        }
    }

    impl ::std::convert::From<::fbthrift::NonthrowingFunctionError> for DoBlandExn {
        fn from(err: ::fbthrift::NonthrowingFunctionError) -> Self {
            match err {
                ::fbthrift::NonthrowingFunctionError::ApplicationException(aexn) => DoBlandExn::ApplicationException(aexn),
                ::fbthrift::NonthrowingFunctionError::ThriftError(err) => DoBlandExn::ApplicationException(::fbthrift::ApplicationException {
                    message: err.to_string(),
                    type_: ::fbthrift::ApplicationExceptionErrorCode::InternalError,
                }),
            }
        }
    }

    impl ::std::convert::From<::fbthrift::ApplicationException> for DoBlandExn {
        fn from(exn: ::fbthrift::ApplicationException) -> Self {
            Self::ApplicationException(exn)
        }
    }

    impl ::fbthrift::ExceptionInfo for DoBlandExn {
        fn exn_name(&self) -> &'static str {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_name(),
            }
        }

        fn exn_value(&self) -> String {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_value(),
            }
        }

        fn exn_is_declared(&self) -> bool {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_is_declared(),
            }
        }
    }

    impl ::fbthrift::ResultInfo for DoBlandExn {
        fn result_type(&self) -> ::fbthrift::ResultType {
            match self {
                Self::ApplicationException(_aexn) => ::fbthrift::ResultType::Exception,
            }
        }
    }

    impl<P> ::fbthrift::Serialize<P> for DoBlandExn
    where
        P: ::fbthrift::ProtocolWriter,
    {
        fn write(&self, p: &mut P) {
            ::fbthrift::help::SerializeExn::write_result(Err(self), p, "doBland");
        }
    }

    impl ::fbthrift::help::SerializeExn for DoBlandExn {
        type Success = ();

        fn write_result<P>(
            res: ::std::result::Result<&Self::Success, &Self>,
            p: &mut P,
            function_name: &'static str,
        )
        where
            P: ::fbthrift::ProtocolWriter,
        {
            if let ::std::result::Result::Err(Self::ApplicationException(aexn)) = res {
                ::fbthrift::Serialize::write(aexn, p);
                return;
            }
            p.write_struct_begin(function_name);
            match res {
                ::std::result::Result::Ok(_success) => {
                    p.write_field_begin("Success", ::fbthrift::TType::Void, 0i16);
                    ::fbthrift::Serialize::write(_success, p);
                    p.write_field_end();
                }

                ::std::result::Result::Err(Self::ApplicationException(_aexn)) => unreachable!(),
            }
            p.write_field_stop();
            p.write_struct_end();
        }
    }

    #[derive(Clone, Debug)]
    pub enum DoRaiseExn {
        b(crate::types::Banal),        f(crate::types::Fiery),        s(crate::types::Serious),
        ApplicationException(::fbthrift::ApplicationException),
    }

    impl ::std::convert::From<crate::types::Banal> for DoRaiseExn {
        fn from(exn: crate::types::Banal) -> Self {
            Self::b(exn)
        }
    }

    impl ::std::convert::From<crate::types::Fiery> for DoRaiseExn {
        fn from(exn: crate::types::Fiery) -> Self {
            Self::f(exn)
        }
    }

    impl ::std::convert::From<crate::types::Serious> for DoRaiseExn {
        fn from(exn: crate::types::Serious) -> Self {
            Self::s(exn)
        }
    }


    impl ::std::convert::From<::fbthrift::ApplicationException> for DoRaiseExn {
        fn from(exn: ::fbthrift::ApplicationException) -> Self {
            Self::ApplicationException(exn)
        }
    }

    impl ::fbthrift::ExceptionInfo for DoRaiseExn {
        fn exn_name(&self) -> &'static str {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_name(),
                Self::b(exn) => exn.exn_name(),
                Self::f(exn) => exn.exn_name(),
                Self::s(exn) => exn.exn_name(),
            }
        }

        fn exn_value(&self) -> String {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_value(),
                Self::b(exn) => exn.exn_value(),
                Self::f(exn) => exn.exn_value(),
                Self::s(exn) => exn.exn_value(),
            }
        }

        fn exn_is_declared(&self) -> bool {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_is_declared(),
                Self::b(exn) => exn.exn_is_declared(),
                Self::f(exn) => exn.exn_is_declared(),
                Self::s(exn) => exn.exn_is_declared(),
            }
        }
    }

    impl ::fbthrift::ResultInfo for DoRaiseExn {
        fn result_type(&self) -> ::fbthrift::ResultType {
            match self {
                Self::ApplicationException(_aexn) => ::fbthrift::ResultType::Exception,
                Self::b(_exn) => fbthrift::ResultType::Error,
                Self::f(_exn) => fbthrift::ResultType::Error,
                Self::s(_exn) => fbthrift::ResultType::Error,
            }
        }
    }

    impl<P> ::fbthrift::Serialize<P> for DoRaiseExn
    where
        P: ::fbthrift::ProtocolWriter,
    {
        fn write(&self, p: &mut P) {
            ::fbthrift::help::SerializeExn::write_result(Err(self), p, "doRaise");
        }
    }

    impl ::fbthrift::help::SerializeExn for DoRaiseExn {
        type Success = ();

        fn write_result<P>(
            res: ::std::result::Result<&Self::Success, &Self>,
            p: &mut P,
            function_name: &'static str,
        )
        where
            P: ::fbthrift::ProtocolWriter,
        {
            if let ::std::result::Result::Err(Self::ApplicationException(aexn)) = res {
                ::fbthrift::Serialize::write(aexn, p);
                return;
            }
            p.write_struct_begin(function_name);
            match res {
                ::std::result::Result::Ok(_success) => {
                    p.write_field_begin("Success", ::fbthrift::TType::Void, 0i16);
                    ::fbthrift::Serialize::write(_success, p);
                    p.write_field_end();
                }
                ::std::result::Result::Err(Self::b(inner)) => {
                    p.write_field_begin(
                        "b",
                        ::fbthrift::TType::Struct,
                        1,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }                ::std::result::Result::Err(Self::f(inner)) => {
                    p.write_field_begin(
                        "f",
                        ::fbthrift::TType::Struct,
                        2,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }                ::std::result::Result::Err(Self::s(inner)) => {
                    p.write_field_begin(
                        "s",
                        ::fbthrift::TType::Struct,
                        3,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }
                ::std::result::Result::Err(Self::ApplicationException(_aexn)) => unreachable!(),
            }
            p.write_field_stop();
            p.write_struct_end();
        }
    }

    #[derive(Clone, Debug)]
    pub enum Get200Exn {

        ApplicationException(::fbthrift::ApplicationException),
    }

    impl ::std::convert::From<Get200Exn> for ::fbthrift::NonthrowingFunctionError {
        fn from(err: Get200Exn) -> Self {
            match err {
                Get200Exn::ApplicationException(aexn) => ::fbthrift::NonthrowingFunctionError::ApplicationException(aexn),
            }
        }
    }

    impl ::std::convert::From<::fbthrift::NonthrowingFunctionError> for Get200Exn {
        fn from(err: ::fbthrift::NonthrowingFunctionError) -> Self {
            match err {
                ::fbthrift::NonthrowingFunctionError::ApplicationException(aexn) => Get200Exn::ApplicationException(aexn),
                ::fbthrift::NonthrowingFunctionError::ThriftError(err) => Get200Exn::ApplicationException(::fbthrift::ApplicationException {
                    message: err.to_string(),
                    type_: ::fbthrift::ApplicationExceptionErrorCode::InternalError,
                }),
            }
        }
    }

    impl ::std::convert::From<::fbthrift::ApplicationException> for Get200Exn {
        fn from(exn: ::fbthrift::ApplicationException) -> Self {
            Self::ApplicationException(exn)
        }
    }

    impl ::fbthrift::ExceptionInfo for Get200Exn {
        fn exn_name(&self) -> &'static str {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_name(),
            }
        }

        fn exn_value(&self) -> String {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_value(),
            }
        }

        fn exn_is_declared(&self) -> bool {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_is_declared(),
            }
        }
    }

    impl ::fbthrift::ResultInfo for Get200Exn {
        fn result_type(&self) -> ::fbthrift::ResultType {
            match self {
                Self::ApplicationException(_aexn) => ::fbthrift::ResultType::Exception,
            }
        }
    }

    impl<P> ::fbthrift::Serialize<P> for Get200Exn
    where
        P: ::fbthrift::ProtocolWriter,
    {
        fn write(&self, p: &mut P) {
            ::fbthrift::help::SerializeExn::write_result(Err(self), p, "get200");
        }
    }

    impl ::fbthrift::help::SerializeExn for Get200Exn {
        type Success = ::std::string::String;

        fn write_result<P>(
            res: ::std::result::Result<&Self::Success, &Self>,
            p: &mut P,
            function_name: &'static str,
        )
        where
            P: ::fbthrift::ProtocolWriter,
        {
            if let ::std::result::Result::Err(Self::ApplicationException(aexn)) = res {
                ::fbthrift::Serialize::write(aexn, p);
                return;
            }
            p.write_struct_begin(function_name);
            match res {
                ::std::result::Result::Ok(_success) => {
                    p.write_field_begin("Success", ::fbthrift::TType::String, 0i16);
                    ::fbthrift::Serialize::write(_success, p);
                    p.write_field_end();
                }

                ::std::result::Result::Err(Self::ApplicationException(_aexn)) => unreachable!(),
            }
            p.write_field_stop();
            p.write_struct_end();
        }
    }

    #[derive(Clone, Debug)]
    pub enum Get500Exn {
        f(crate::types::Fiery),        b(crate::types::Banal),        s(crate::types::Serious),
        ApplicationException(::fbthrift::ApplicationException),
    }

    impl ::std::convert::From<crate::types::Fiery> for Get500Exn {
        fn from(exn: crate::types::Fiery) -> Self {
            Self::f(exn)
        }
    }

    impl ::std::convert::From<crate::types::Banal> for Get500Exn {
        fn from(exn: crate::types::Banal) -> Self {
            Self::b(exn)
        }
    }

    impl ::std::convert::From<crate::types::Serious> for Get500Exn {
        fn from(exn: crate::types::Serious) -> Self {
            Self::s(exn)
        }
    }


    impl ::std::convert::From<::fbthrift::ApplicationException> for Get500Exn {
        fn from(exn: ::fbthrift::ApplicationException) -> Self {
            Self::ApplicationException(exn)
        }
    }

    impl ::fbthrift::ExceptionInfo for Get500Exn {
        fn exn_name(&self) -> &'static str {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_name(),
                Self::f(exn) => exn.exn_name(),
                Self::b(exn) => exn.exn_name(),
                Self::s(exn) => exn.exn_name(),
            }
        }

        fn exn_value(&self) -> String {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_value(),
                Self::f(exn) => exn.exn_value(),
                Self::b(exn) => exn.exn_value(),
                Self::s(exn) => exn.exn_value(),
            }
        }

        fn exn_is_declared(&self) -> bool {
            match self {
                Self::ApplicationException(aexn) => aexn.exn_is_declared(),
                Self::f(exn) => exn.exn_is_declared(),
                Self::b(exn) => exn.exn_is_declared(),
                Self::s(exn) => exn.exn_is_declared(),
            }
        }
    }

    impl ::fbthrift::ResultInfo for Get500Exn {
        fn result_type(&self) -> ::fbthrift::ResultType {
            match self {
                Self::ApplicationException(_aexn) => ::fbthrift::ResultType::Exception,
                Self::f(_exn) => fbthrift::ResultType::Error,
                Self::b(_exn) => fbthrift::ResultType::Error,
                Self::s(_exn) => fbthrift::ResultType::Error,
            }
        }
    }

    impl<P> ::fbthrift::Serialize<P> for Get500Exn
    where
        P: ::fbthrift::ProtocolWriter,
    {
        fn write(&self, p: &mut P) {
            ::fbthrift::help::SerializeExn::write_result(Err(self), p, "get500");
        }
    }

    impl ::fbthrift::help::SerializeExn for Get500Exn {
        type Success = ::std::string::String;

        fn write_result<P>(
            res: ::std::result::Result<&Self::Success, &Self>,
            p: &mut P,
            function_name: &'static str,
        )
        where
            P: ::fbthrift::ProtocolWriter,
        {
            if let ::std::result::Result::Err(Self::ApplicationException(aexn)) = res {
                ::fbthrift::Serialize::write(aexn, p);
                return;
            }
            p.write_struct_begin(function_name);
            match res {
                ::std::result::Result::Ok(_success) => {
                    p.write_field_begin("Success", ::fbthrift::TType::String, 0i16);
                    ::fbthrift::Serialize::write(_success, p);
                    p.write_field_end();
                }
                ::std::result::Result::Err(Self::f(inner)) => {
                    p.write_field_begin(
                        "f",
                        ::fbthrift::TType::Struct,
                        1,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }                ::std::result::Result::Err(Self::b(inner)) => {
                    p.write_field_begin(
                        "b",
                        ::fbthrift::TType::Struct,
                        2,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }                ::std::result::Result::Err(Self::s(inner)) => {
                    p.write_field_begin(
                        "s",
                        ::fbthrift::TType::Struct,
                        3,
                    );
                    ::fbthrift::Serialize::write(inner, p);
                    p.write_field_end();
                }
                ::std::result::Result::Err(Self::ApplicationException(_aexn)) => unreachable!(),
            }
            p.write_field_stop();
            p.write_struct_end();
        }
    }
}
