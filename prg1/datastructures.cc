// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <QDebug>

#include <iostream>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
   auto range = end-start;
   ++range;

   auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

   return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
   // Replace this with your implementation
}

Datastructures::~Datastructures()
{
    beacon_id_vec.clear();
        for(auto pair: beacon_map) {
            pair.second = nullptr;
        }
        beacon_map.clear();
}

int Datastructures::beacon_count()
{
     return static_cast<int>(beacon_id_vec.size());
};

void Datastructures::clear_beacons()
{
    beacon_id_vec.clear();

        for(auto pair: beacon_map) {
            pair.second = nullptr;
        }
        beacon_map.clear();
}

std::vector<BeaconID> Datastructures::all_beacons()
{
    //if(!beacon_id_vec.empty()){
           //return beacon_id_vec;
       //}
   //std::cerr<<beacon_id_vec.size()<<std::endl;
   //beacon_id_vec.clear();
    //for (std::map<BeaconID, Beacon_property* >::iterator element=beacon_map.begin(); element!=beacon_map.end(); ++element)
       //{
           //BeaconID vec_beacon_id = element->second->beacon_id;
           //beacon_id_vec.insert(beacon_id_vec.begin(), vec_beacon_id);
       //}
    return beacon_id_vec;
}

bool Datastructures::add_beacon(BeaconID beacon_id, std::string const& beacon_name, Coord xy, Color color)
{
   Beacon_property* add_new_beacon = new Beacon_property({
       beacon_id,
       beacon_name,
       xy,
       color,
       nullptr
   });
   beacon_map.insert({beacon_id, add_new_beacon});
   if(beacon_map.find(beacon_id) != beacon_map.end()){
       beacon_id_vec.push_back(beacon_id);
   }
   return true;
}

std::string Datastructures::get_name(BeaconID beacon_id)
{
   if(beacon_map[beacon_id]){
           const Beacon_property get_beacon_address = *beacon_map[beacon_id];
           return get_beacon_address.beacon_name;
       } else {
           return NO_NAME;
       }
}

Coord Datastructures::get_coordinates(BeaconID beacon_id)
{
   if(beacon_map[beacon_id]){
           const Beacon_property get_beacon_address = *beacon_map[beacon_id];
           return get_beacon_address.xy;
       } else {
           return NO_COORD;
       }
}

Color Datastructures::get_color(BeaconID beacon_id)
{
   if(beacon_map[beacon_id]){
           const Beacon_property get_beacon_address = *beacon_map[beacon_id];
           return get_beacon_address.color;
       } else {
           return NO_COLOR;
       }
}


std::vector<BeaconID> Datastructures::beacons_alphabetically()
{
   sort( beacon_id_vec.begin(), beacon_id_vec.end(), [this]( BeaconID & x, BeaconID & y )
       {
           return this->nameSort(x, y);
       });

       return beacon_id_vec;
}

bool Datastructures::nameSort(BeaconID x, BeaconID y)
{
   return beacon_map[x]->beacon_name < beacon_map[y]->beacon_name;
}

std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
   sort( beacon_id_vec.begin(), beacon_id_vec.end(), [this]( BeaconID & x, BeaconID & y )
       {
           return this->colorSort(x, y);
       });

       return beacon_id_vec;
}

bool Datastructures::colorSort(BeaconID x, BeaconID y)
{
   int c1 = 3*beacon_map[x]->color.r+6*beacon_map[x]->color.g+beacon_map[x]->color.b;
   int c2 = 3*beacon_map[y]->color.r+6*beacon_map[y]->color.g+beacon_map[y]->color.b;
   return c1 < c2;
}


BeaconID Datastructures::min_brightness()
{
   if(!beacon_id_vec.empty()){
           sort( beacon_id_vec.begin(), beacon_id_vec.end(), [this]( BeaconID & x, BeaconID & y )
           {
               return this->colorSort(x, y);
           });
           return beacon_id_vec.front();
       }
       return NO_ID;
}

BeaconID Datastructures::max_brightness()
{
   if(!beacon_id_vec.empty()){
           sort( beacon_id_vec.begin(), beacon_id_vec.end(), [this]( BeaconID & x, BeaconID & y )
           {
               return this->colorSort(x, y);
           });
           return beacon_id_vec.back();
       }
       return NO_ID;
}

std::vector<BeaconID> Datastructures::find_beacons(std::string const& beacon_name)
{
   std::vector<BeaconID> temp_beacon_ids;
   std::vector<BeaconID>::iterator ptr;
   for (ptr = beacon_id_vec.begin(); ptr < beacon_id_vec.end(); ptr++){
       if(beacon_map[*ptr]->beacon_name == beacon_name){
           temp_beacon_ids.push_back(*ptr);
       }
   }
   return temp_beacon_ids;}

bool Datastructures::change_beacon_name(BeaconID id, const std::string& newname)
{
   std::map<BeaconID, Beacon_property* >::iterator it;
       it = beacon_map.find(id);
       if (it != beacon_map.end()){
           it->second->beacon_name = newname;
           return true;
       } else {
           return false;
       }
}

bool Datastructures::change_beacon_color(BeaconID id, Color newcolor)
{
   std::map< BeaconID, Beacon_property* >::iterator iterator;
       iterator = beacon_map.find(id);
       if (iterator != beacon_map.end()){
           iterator->second->color = newcolor;
           return true;
       } else {
           return false;
       }
}

bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
   std::map<BeaconID, Beacon_property* >::iterator becon_it;
       std::map<BeaconID, Beacon_property* >::iterator beacon_it_2;
       becon_it = beacon_map.find(sourceid);
       beacon_it_2 = beacon_map.find(targetid);
       if (becon_it != beacon_map.end() && beacon_it_2 != beacon_map.end()){
           Beacon_property * beacon_info = beacon_map[targetid];
           if(becon_it->second->address_id == nullptr){
               becon_it->second->address_id = beacon_info;
           }
           return true;
       } else {
           return false;
       }
}

std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{
   std::vector<BeaconID> beacon_light_beam;
       std::vector<BeaconID>::iterator beacon_it;

       for (beacon_it = beacon_id_vec.begin(); beacon_it < beacon_id_vec.end(); beacon_it++){
           Beacon_property beacon = *beacon_map[*beacon_it];
           Beacon_property * targetBeacon = beacon.address_id;
           if(targetBeacon != nullptr  && targetBeacon->beacon_id == id){
               beacon_light_beam.push_back(beacon.beacon_id);
           }
       }
       if(beacon_light_beam.size()>0){
           return beacon_light_beam;
       } else {
           return {{NO_ID}};
       }}

std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
   std::vector<BeaconID> new_beacon_vec;
       new_beacon_vec.push_back(id);
       Beacon_property beacon = *beacon_map[id];
       Beacon_property * targetBeacon = beacon.address_id;
       while(targetBeacon)
       {
         new_beacon_vec.push_back(targetBeacon->beacon_id);
         targetBeacon = targetBeacon->address_id;
       }
       return new_beacon_vec;
}

bool Datastructures::remove_beacon(BeaconID /*id*/)
{
   return false;
}

std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID /*id*/)
{
   return {{NO_ID}};
}

Color Datastructures::total_color(BeaconID /*id*/)
{
   return NO_COLOR;
}


