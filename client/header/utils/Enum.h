#ifndef ENUM_H
#define ENUM_H

#include <QMetaEnum>

template <typename E>
concept EnumTypename = std::is_enum_v<E>;
template <EnumTypename E>
using underlyingType = std::underlying_type_t<E>;

template <EnumTypename E>
class Enum
{
public:
    Enum():data(QMetaEnum::fromType<E>()){
        for (int i = 0; i < data.keyCount(); ++i)
            map.insert(data.key(i), static_cast<E>(data.value(i)));
    }

    [[nodiscard("Returns MemberCount Only")]]
    std::size_t Count() const
    { return data.keyCount(); }

    [[nodiscard("Returns Value Only")]]
    E Map(const QByteArray& name)
    { return map.value(name, static_cast<E>(-1)); }

    [[nodiscard("Returns Name Only")]]
    QByteArray Name (E value)
    { return data.valueToKey(static_cast<underlyingType<E>>(value)); }

    auto Data()
    { return map.asKeyValueRange(); }
private:
    QMetaEnum data;
    QMap<QByteArray, E> map;
};

namespace Reflex
{
    template <EnumTypename E>
    [[nodiscard("Returns EnumMemberCount Only")]]
    std::size_t Enum_Count()
    { return QMetaEnum::fromType<E>().keyCount(); }

    template <EnumTypename E>
    [[nodiscard("Returns EnumValue Only")]]
    E Enum_Value(const QByteArray& name)
    { return QMetaEnum::fromType<E>().keyToValue(name); }

    template <EnumTypename E>
    [[nodiscard("Returns EnumName Only")]]
    QByteArray Enum_Name(const E value)
    { return QMetaEnum::fromType<E>().valueToKey(value); }
}

#endif // ENUM_H
