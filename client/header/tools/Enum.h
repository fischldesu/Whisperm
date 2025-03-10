#ifndef ENUM_H
#define ENUM_H

#include "multiplatform.hpp"

template <typename E>
concept EnumTypename = std::is_enum_v<E>;

class Enum
{
public:
    template <EnumTypename E>
    static constexpr QString Reflex_TypeName()
    {
        auto name = Reflex::TemplateName<E>();
        if(System::GetCompiler() == System::Compiler::MSVC)
            return name.mid(5, name.size() - 5);
        return name;
    }
};

#endif // ENUM_H
