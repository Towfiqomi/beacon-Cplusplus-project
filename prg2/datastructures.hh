// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <list>
// Type for beacon IDs
using BeaconID = std::string;

// Return value for cases where required beacon was not found
BeaconID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for color (RGB)
struct Color
{
    int r = NO_VALUE;
    int g = NO_VALUE;
    int b = NO_VALUE;
};

// Equality and non-equality comparisons for Colors
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1==c2); }

// Return value for cases where color was not found
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost (used only in the second assignment)
using Cost = int;

// Return value for cases where cost is unknown
Cost const NO_COST = NO_VALUE;


// This is the class you are supposed to implement
struct Beacon_property
{
    BeaconID beacon_id;
    std::string beacon_name;
    Coord xy;
    Color color;
    Beacon_property * address_id;

};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    int beacon_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_beacons();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> all_beacons();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_beacon(BeaconID id, std::string const& name, Coord xy, Color color);

    // Estimate of performance:
    // Short rationale for estimate:
    std::string get_name(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_coordinates(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Color get_color(BeaconID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    BeaconID min_brightness();

    // Estimate of performance:
    // Short rationale for estimate:
    BeaconID max_brightness();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> find_beacons(std::string const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_beacon_name(BeaconID id, std::string const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_beacon_color(BeaconID id, Color newcolor);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> path_outbeam(BeaconID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_beacon(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Color total_color(BeaconID id);

    // Phase 2 operations

    // Estimate of performance: O(n log n)
    // Short rationale for estimate:
    // std::sort average time complexity is n log n
    // Unordered map iteration complexity is O(n)
    std::vector<Coord> all_xpoints();

    // Estimate of performance: O(n log n)
    // Short rationale for estimate:
    // Find in vector time complexity is O(n)
    // vector insertion O(n)
    // all other can be ignored
    bool add_fibre(Coord xpoint1, Coord xpoint2, Cost cost);

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    // Searching in unordered map worst case complexity O(n)
    // Accessing value using key in unordered map complexity O(n)
    std::vector<std::pair<Coord, Cost>> get_fibres_from(Coord xpoint);

    // Estimate of performance: O(1)
    // Short rationale for estimate:
    // vector return time complexity O(1)
    std::vector<std::pair<Coord, Coord>> all_fibres();

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    // Searching in unordered map worst case complexity O(n)
    // vector iteration time complexity O(n)
    // vector deletion time complexity is O(n)
    bool remove_fibre(Coord xpoint1, Coord xpoint2);

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    void clear_fibres();

    // Estimate of performance: O(n)
    // Short rationale for estimate:
    // Find in unordered map time complexity is O(n)
    // Insertion in unordered map O(n)
    // Unordered map iteration O(n)
    // Vector push_back operation O(1)
    // Accessing value using key in unordered map complexity O(n)
    // vector deletion O(1)
    std::vector<std::pair<Coord, Cost>> route_any(Coord fromxpoint, Coord toxpoint);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Cost>> route_least_xpoints(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Cost>> route_fastest(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> route_fibre_cycle(Coord startxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    Cost trim_fibre_network();

private:
    std::map< BeaconID, Beacon_property* > beacon_map;
    std::vector<BeaconID> beacon_id_vec;
    bool nameSort(BeaconID x, BeaconID y);


    std::unordered_map<Coord, std::vector<std::pair<Coord, Cost>>, CoordHash> nodes_;
    std::vector<std::pair<Coord, Coord>> path_list_;
};

#endif // DATASTRUCTURES_HH
