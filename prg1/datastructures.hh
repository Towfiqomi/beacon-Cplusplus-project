// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <map>


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

// Beacon data struct

struct Beacon_property {
    BeaconID beacon_id;
    std::string beacon_name;
    Coord xy;
    Color color;
    Beacon_property * address_id;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:O(N)
    // Short rationale for estimate:
    // The function counts every element in vector one by one by one.
    // For one element the function iterate one time. For n number of elements O(n) time the function will iterate.
    // For that the estimate of performance will be O(N)
    int beacon_count();

    // Estimate of performance:3 O(N)
    // Short rationale for estimate:
    // The function performs a for loop of map.
    // For every n number of elements the loop will iterate O(N) times.
    // The function removes the vector elements so the worst case will be O(N).
    // Also the function removes the element from the map container, so the complexity will be linear O(N).
    // So considering the three operation the function is performing the estimation of performance will be 3 O(N)
    void clear_beacons();

    // Estimate of performance:O(1)
    // Short rationale for estimate:
    // The function returns the vector one time.
    // For that the estimation of performance will be O(1)
    std::vector<BeaconID> all_beacons();

    // Estimate of performance:2 O(log N)
    // Short rationale for estimate:
    // The peroforms two map operations.
    // It insert a new element in map container and does a searching inside if condition.
    // Time complexity for map insertion in worst and best case scenario is O(log N).
    // Also the time complexity of map searching is O(log N)
    // There is also an vector push back opration whose time complexity is O(1).
    // We can ignore the vector push back timecomplexity.
    // So the estimate performance of this function is 2 O(log N)
    bool add_beacon(BeaconID id, std::string const& name, Coord xy, Color color);

    // Estimate of performance:O(log N)
    // Short rationale for estimate:
    // The function does a map search operation which time complexity is O(log N)
    // So the estimate time complexity will be O(log N)
    std::string get_name(BeaconID id);

    // Estimate of performance:O(log N)
    // Short rationale for estimate:
    // The function does a map search operation which time complexity is O(log N)
    // So the estimate time complexity will be O(logN)
    Coord get_coordinates(BeaconID id);

    // Estimate of performance:O(log N)
    // Short rationale for estimate:
    // The function does a map search operation which time complexity is O(log N)
    // So the estimate time complexity will be O(log N)
    Color get_color(BeaconID id);

    // Estimate of performance:O(N log N)
    // Short rationale for estimate:
    // I have used the STL sort (std::sort) for this function
    // std::sort has average case time complexity is O(N log N).
    // So the estimate performance of this function will be O(N log N).
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance:O(N log N)
    // Short rationale for estimate:
    // I have used the STL sort (std::sort) for this function
    // std::sort has average case time complexity is O(N log N).
    // So the estimate performance of this function will be O(N log N).
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance:O(1) + O(N log N) + O(1)
    // Short rationale for estimate:
    // First the function checks if there is an element in the vector.
    // The operation runs one time so the complexity will be O(1).
    // For std::sort operation the average case time complexity is O(N log N).
    // Later the function returns the last element in the list.
    // For vector back operation the time complexity is O(1)
    // So considering the three operation the function is performing the estimation of performance will be
    // O(1) + O(N log N) + O(1)
    BeaconID min_brightness();

    // Estimate of performance:O(1) + O(N log N) + O(1)
    // Short rationale for estimate:
    // First the function checks if there is an element in the vector.
    // The operation runs one time so the complexity will be O(1).
    // For std::sort operation the average case time complexity is O(N log N).
    // Later the function returns the last element in the list.
    // For vector back operation the time complexity is O(1)
    // So considering the three operation the function is performing the estimation of performance will be
    // O(1) + O(N log N) + O(1)
    BeaconID max_brightness();

    // Estimate of performance:O(N)
    // Short rationale for estimate:
    // The function performs a for loop operation for vector, for that the time complexity will be O(N).
    // There is also an vector push back opration whose time complexity is O(1).
    // We can ignore the vector push back time complexity.
    // So the estimate performance of this function is O(N)
    std::vector<BeaconID> find_beacons(std::string const& name);

    // Estimate of performance:O(log N)
    // Short rationale for estimate:
    // The function does a map search operation which time complexity is O(log N).
    // There is also a checker without any looping, so the time complexity is O(1).
    // So ignoring O(1), the estimate performance of this operation will be O(log N).
    bool change_beacon_name(BeaconID id, std::string const& newname);

    // Estimate of performance:O(log n)
    // Short rationale for estimate:
    // The function does a map search operation which time complexity is O(log N).
    // There is also a checker without any looping, so the time complexity is O(1).
    // So ignoring O(1), the estimate performance of this operation will be O(log N).
    bool change_beacon_color(BeaconID id, Color newcolor);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:2 O(log N) + O(N)
    // Short rationale for estimate:
    // The function does a map search operation two times which time complexity will be 2 O(log N).
    // There is also searches nth element of map which time complexity is O(N).
    // So, considering the both above operation the function performs,
    // The enstimate performance of this function will be 2 O(log N) + O(N).
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance:O(N)
    // Short rationale for estimate:
    // The function performs a for loop operation for vector, for that the time complexity will be O(N).
    // The function also performs an one time operation of checking the size of the vector,
    // Whose time complexity will be O(1).
    // So ignoring O(1), the estimate performance of this operation will be O(N).
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance: 2 O(N)
    // Short rationale for estimate:
    // The function performs a while loop for vector which time complexity is O(N).
    // There is also searches nth element of map which time complexity is O(N).
    // There is also a O(1) operation of vector push back.
    // So ignoring O(1), the estimate performance of this operation will be 2 O(N).
    std::vector<BeaconID> path_outbeam(BeaconID id);


    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_beacon(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Color total_color(BeaconID id);

private:
     std::map< BeaconID, Beacon_property* > beacon_map;
     std::vector<BeaconID> beacon_id_vec;
     bool nameSort(BeaconID x, BeaconID y);
     bool colorSort(BeaconID x, BeaconID y);

};

#endif // DATASTRUCTURES_HH
