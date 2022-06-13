#include <string>
#include <vector>

static std::vector <std::string> country_prefix = {
    "Ru", "Ge", "Uni", "Fr", "It",
    "Sp", "Uk", "Po", "Ro", "Ne",
    "Be", "Sw", "De", "Fi", "No",
};

static std::vector <std::string> country_suffix = {
    "land", "stan", "ia", "ny", "burg",
};

static std::vector <std::string> country_middle = {
    "ma", "an", "it", "te", "al",
    "pa", "ra", "ai", "la", "ol",
    "th", "er",
};

static std::vector <std::string> city_names = {
    "Tokyo", "Delhi", "Shanghai", "Sao Paulo", "Cairo",
    "Mumbai", "Beijing", "Dhaka", "Osaka", "New York", 
    "Karachi", "Buenos Aires", "Chongqing", "Istanbul",
};

namespace iso {
    enum class GenerationType {
        COUNTRY,
        CITY,
    };

    std::string generate(GenerationType type, int iterations);
}