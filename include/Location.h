#include <utility>


struct CityLocation {
    double x, y;

};

struct LessX {
    bool operator()(const CityLocation& l, const CityLocation& r) const {
        return (l.x < r.x);
    }
};


struct LessY {
    bool operator()(const CityLocation& l, const CityLocation& r) const {
        return (l.y < r.y);
    }
};


struct CompareCityByX {
    using cityPair = std::pair<CityLocation, std::string>;

    bool operator()(const cityPair& l, const cityPair& r) const {
        return (l.first.x < r.first.x);
    }
};