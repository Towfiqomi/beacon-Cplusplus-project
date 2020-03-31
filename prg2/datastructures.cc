// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

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
 //
}

Datastructures::~Datastructures()
{
    beacon_id_vec.clear();
    for(auto pair: beacon_map)
    {
       pair.second = nullptr;
     }
    beacon_map.clear();

}

int Datastructures::beacon_count()
{
    return static_cast<int>(beacon_id_vec.size());

}

void Datastructures::clear_beacons()
{
    beacon_id_vec.clear();
        for(auto pair: beacon_map)
        {
            pair.second = nullptr;
        }
        beacon_map.clear();

}

std::vector<BeaconID> Datastructures::all_beacons()
{
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
    // Replace this with your implementation
    return {};
}

BeaconID Datastructures::min_brightness()
{
    // Replace this with your implementation
    return NO_ID;
}

BeaconID Datastructures::max_brightness()
{
    // Replace this with your implementation
    return NO_ID;
}

std::vector<BeaconID> Datastructures::find_beacons(std::string const& beacon_name)
{
    // Replace this with your implementation
    std::vector<BeaconID> temp_beacon_ids;
    std::vector<BeaconID>::iterator ptr;
    for (ptr = beacon_id_vec.begin(); ptr < beacon_id_vec.end(); ptr++){
        if(beacon_map[*ptr]->beacon_name == beacon_name){
            temp_beacon_ids.push_back(*ptr);
        }
    }
    return temp_beacon_ids;

}

bool Datastructures::change_beacon_name(BeaconID /*id*/, const std::string& /*newname*/)
{
    // Replace this with your implementation
    return false;
}

bool Datastructures::change_beacon_color(BeaconID /*id*/, Color /*newcolor*/)
{
    // Replace this with your implementation
    return false;
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
        }

}

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
    // Replace this with your implementation
    return false;
}

std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID /*id*/)
{
    // Replace this with your implementation
    return {NO_ID};
}

Color Datastructures::total_color(BeaconID /*id*/)
{
    return NO_COLOR;
}

std::vector<Coord> Datastructures::all_xpoints()
{
    std::vector<Coord> all_xpoints;
    for (auto const& element : nodes_)
    {
        all_xpoints.push_back(element.first);
    }
    std::sort(all_xpoints.begin(), all_xpoints.end());
    return all_xpoints;
}

bool Datastructures::add_fibre(Coord xpoint1, Coord xpoint2, Cost cost)
{
    std::vector<std::pair<Coord, Cost>> temp_vec_ = nodes_[xpoint1];
    auto iter = std::find_if(temp_vec_.begin(), temp_vec_.end(),[xpoint2](const std::pair<Coord, Cost> vector){
        return vector.first == xpoint2;
    });

    if(iter == temp_vec_.end())
    {
        nodes_[xpoint1].push_back(std::make_pair(xpoint2,cost));
        nodes_[xpoint2].push_back(std::make_pair(xpoint1,cost));
        path_list_.push_back(std::make_pair(xpoint1, xpoint2));
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::pair<Coord, Cost> > Datastructures::get_fibres_from(Coord xpoint)
{
    std::vector<std::pair<Coord, Cost> > fibres_from;
    auto it = nodes_.find(xpoint);
    if(it != nodes_.end())
    {
        fibres_from = nodes_[xpoint];
    }
    std::sort(fibres_from.begin(), fibres_from.end());
    return fibres_from;
}

std::vector<std::pair<Coord, Coord> > Datastructures::all_fibres()
{
    std::sort(path_list_.begin(), path_list_.end());
    return path_list_;
}

bool Datastructures::remove_fibre(Coord xpoint1, Coord xpoint2)
{

    std::pair<Coord, Coord> node_pair;
    if(xpoint1 < xpoint2)
    {
        node_pair = std::make_pair(xpoint1, xpoint2);
    } else {
        node_pair = std::make_pair(xpoint2, xpoint1);
    }
    auto iter = std::find_if(path_list_.begin(), path_list_.end(),[&node_pair](const std::pair<Coord, Coord> vector){
        return vector == node_pair;
    });

    if(iter != path_list_.end()){
        path_list_.erase(iter);

        std::vector<std::pair<Coord, Cost> > from_xpoint1_vector;
        from_xpoint1_vector = nodes_[xpoint1];
        std::pair<Coord, Cost> pair_1 = std::make_pair(xpoint1, 0);
        std::vector<std::pair<Coord, Cost> >::iterator iter = std::find_if(from_xpoint1_vector.begin(), from_xpoint1_vector.end(),[pair_1](const std::pair<Coord, Cost> vector){
            return vector.first == pair_1.first;
        });
        from_xpoint1_vector.erase(iter);
        nodes_[xpoint1] = from_xpoint1_vector;

        std::vector<std::pair<Coord, Cost> > from_xpoint2_vector;
        from_xpoint2_vector = nodes_[xpoint2];
        std::pair<Coord, Cost> pair_2 = std::make_pair(xpoint2, 0);
        std::vector<std::pair<Coord, Cost> >::iterator iter_ = std::find_if(from_xpoint2_vector.begin(), from_xpoint2_vector.end(),[pair_2](const std::pair<Coord, Cost> vector){
            return vector.first == pair_2.first;
        });
        from_xpoint2_vector.erase(iter_);
        nodes_[xpoint2] = from_xpoint2_vector;

        return true;
    }
    return false;
}

void Datastructures::clear_fibres()
{
    nodes_.clear();
    path_list_.clear();
}

std::vector<std::pair<Coord, Cost> > Datastructures::route_any(Coord fromxpoint, Coord toxpoint)
{
    std::vector<std::pair<Coord,Cost> > get_routes;
    auto iter1 = nodes_.find(fromxpoint);
    auto iter2 = nodes_.find(toxpoint);
    if(iter1 != nodes_.end() && iter2 != nodes_.end())
    {
        std::unordered_map<Coord, int, CoordHash> map_of_visited_path;
        std::unordered_map<Coord, std::pair<Coord, Cost>, CoordHash> map_of_the_nodes;

        bool destination_found = false;

        for (auto const& element : nodes_)
        {
            map_of_visited_path[element.first] = 0;
            map_of_the_nodes[element.first] = std::pair(NO_COORD, 0);
        }

        std::list<Coord> queue;
        queue.push_back(fromxpoint);
        map_of_visited_path[fromxpoint] = 1;

        while(!queue.empty())
        {
            Coord current_node = queue.front();
            queue.pop_front();
            for (auto const& node : nodes_[current_node])
            {
                if(map_of_visited_path[node.first] == 0)
                {
                    map_of_visited_path[node.first] = 1;
                    map_of_the_nodes[node.first] = std::pair(current_node, node.second + map_of_the_nodes[current_node].second);
                    queue.push_back(node.first);

                    if(node.first == toxpoint)
                    {
                        destination_found = true;
                        break;
                    }
                }
            }
        }

        if(destination_found == true)
        {
            Coord destination_node = toxpoint;
            while(destination_node != NO_COORD)
            {
                get_routes.push_back(std::pair(destination_node, map_of_the_nodes[destination_node].second));
                destination_node = map_of_the_nodes[destination_node].first;
            }
            std::reverse(get_routes.begin(), get_routes.end());
            return get_routes;

        }
        return get_routes;
    }
    else
    {
        return get_routes;
    }
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    return {};
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord /*fromxpoint*/, Coord /*toxpoint*/)
{
    return {};
}

std::vector<Coord> Datastructures::route_fibre_cycle(Coord /*startxpoint*/)
{
    return {};
}

Cost Datastructures::trim_fibre_network()
{
    return NO_COST;
}
