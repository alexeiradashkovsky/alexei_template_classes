#ifndef STORNG_TYPE_H
#define STRONG_TYPE_H


namespace alexei_prog_snob {

template<typename Tag, typename T>
class StrongType {
public:
    StrongType() : m_value() {}

    explicit StrongType(const T& _value) : m_value(_value) {}

    explicit StrongType(T&& _value) 
    : m_value(std::move(_value)) {}

    explicit operator T&() { return m_value; }

    explicit operator const T& () const { return m_value; }
private:
    T m_value;
};

template<class StrongType>
struct Addition {
    StrongType& operator+=(StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        static_cast<type&>(_lhs) += static_cast<const type&>(_rhs);
        return _lhs;
    }

    StrongType operator+(const StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        return StrongType(static_cast<const type&>(_lhs) + static_cast<const type&>(_rhs));
    }
};

template<class StrongType>
struct Subtraction {
    StrongType& operator-=(StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        static_cast<type&>(_lhs) -= static_cast<const type&>(_rhs);
        return _lhs;
    }

    StrongType operator-(const StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        return StrongType(static_cast<const type&>(_lhs) - static_cast<const type&>(_rhs));
    }
};

template<class StrongType>
struct Multiplication {
    StrongType& operator*=(StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        static_cast<type&>(_lhs) *= static_cast<const type&>(_rhs);
        return _lhs;
    }

    StrongType operator*(const StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        return StrongType(static_cast<const type&>(_lhs) * static_cast<const type&>(_rhs));
    }
};

template<class StrongType>
struct Division {
    StrongType& operator/=(StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        static_cast<type&>(_lhs) /= static_cast<const type&>(_rhs);
        return _lhs;
    }

    StrongType operator/(const StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        return StrongType(static_cast<const type&>(_lhs) / static_cast<const type&>(_rhs));
    }
};

template<class StrongType>
struct Modulo {
    StrongType& operator%=(StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        static_cast<type&>(_lhs) %= static_cast<const type&>(_rhs);
        return _lhs;
    }

    StrongType operator%(const StrongType& _lhs, const StrongType& _rhs) {
        using type = std::underlying_type<StrongType>;
        return StrongType(static_cast<const type&>(_lhs) % static_cast<const type&>(_rhs));
    }
};

} // end namespace alexei_prog_snob

#endif