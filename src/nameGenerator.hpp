#include <string>
#include <vector>

static std::vector <std::string> prefix = {
    "Ru",
    "Ge",
    "Uni",
    "Fr",
    "It",
    "Sp",
    "Uk",
    "Po",
    "Ro",
    "Ne",
    "Be",
    "Sw",
    "De",
    "Fi",
    "No",
};

static std::vector <std::string> suffix = {
    "land",
    "stan",
    "ia",
    "ny",
    "burg",
};

static std::vector <std::string> middle = {
    "ma",
    "an",
    "it",
    "te",
    "al",
    "pa",
    "ra",
    "ai",
    "la",
    "ol",
    "th",
    "er",
};

namespace iso {
    enum class GenerationType {
        COUNTRY,
    };

    std::string generate(GenerationType type, int iterations);
}