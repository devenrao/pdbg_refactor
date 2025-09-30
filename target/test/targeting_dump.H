#include <targeting/association.H>
#include <targeting/common/entitypath.H>
#include <targeting/predicates/predicateattr.H>
#include <targeting/predicates/predicateattrval.H>
#include <targeting/predicates/predicatepostfixexpr.H>
#include <targeting/target.H>
#include <targeting/xmltohb/attributeenums.H>
#include <targeting/xmltohb/attributestructs.H>
#include <targeting/xmltohb/attributetraits.H>
#include <targeting/xmltohb/targAttrIdToName.H>
#include <targetsvc/target_service.H>

#include <iomanip>
#include <iostream>
extern "C"
{
#include <libfdt.h>
}

#include <string>
#include <sstream>
#include <cstdint>

using namespace TARGETING;
template<const ATTRIBUTE_ID A, typename = void>
struct has_typedef_typestdarr : std::false_type {};

template<const ATTRIBUTE_ID A>
struct has_typedef_typestdarr<
    A,
    std::void_t<typename AttributeTraits<A>::TypeStdArr>
> : std::true_type {};

template <typename T>
std::string attrValueToString(const T& value) {
    std::ostringstream oss;

    // std::string
    if constexpr (std::is_same_v<T, std::string>) {
        return value;
    }

    // TARGETING::EntityPath
    else if constexpr (std::is_same_v<T, TARGETING::EntityPath>) {
        return value.toString();
    }

    // TARGETING::HwasState
    else if constexpr (std::is_same_v<T, TARGETING::HwasState>) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(std::addressof(value));
        oss << "[";
        for (size_t i = 0; i < sizeof(TARGETING::HwasState); ++i) {
            if (i > 0) oss << " ";
            oss << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(bytes[i]);
        }
        oss << "]";
        return oss.str();
    }

    // std::array
    else if constexpr (std::is_array_v<T> || (requires { typename T::value_type; })) {
        oss << "[";
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) oss << " ";
            if constexpr (std::is_same_v<typename T::value_type, char>) {
                return std::string(value.data());
            } else {
                oss << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(value[i]);
            }
        }
        oss << "]";
        return oss.str();
    }

    // integral types
    else if constexpr (std::is_integral_v<T>) {
        return std::to_string(value);
    }

    // Default fallback: 
    else {
        oss << value;
        return oss.str();
    }
}

void getAllAttrAndPrint(TargetPtr tgt);

template<const ATTRIBUTE_ID A>
void getAttrAndPrint(Target* tgt) {
    std::string IDstr;

    // Lookup ID in rw and non-rw maps
    if (auto it = g_rwAttrIdToNameMap.find(A); it != g_rwAttrIdToNameMap.end()) {
        IDstr = it->second;
    } else if (auto it = g_nonRwAttrIdToNameMap.find(A); it != g_nonRwAttrIdToNameMap.end()) {
        IDstr = it->second;
    } else {
        IDstr = "UNKNOWN_ATTR";
    }
    try
    {
        if constexpr (has_typedef_typestdarr<A>::value) {
            typename AttributeTraits<A>::TypeStdArr value{};
            value = tgt->getAttrAsArray<A>();
            printf("%-40s :: %s\n", IDstr.c_str(), attrValueToString(value).c_str());
        } else {
            typename AttributeTraits<A>::Type value{};
            value = tgt->getAttr<A>();
            printf("%-40s :: %s\n", IDstr.c_str(), attrValueToString(value).c_str());
        }
    }
    catch(...)
    {
        //no need to print error
    }
}
