// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::binary_search;
using std::max_element;
using std::max;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;
#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";


//std::tuple<unsigned long int,string> mempeak(){
//    ifstream file_stream("/proc/self/status");
//    if( file_stream ){
//        string line;
//        string ID;
//        unsigned long int amnt;
//        string unit;
//        while( getline( file_stream, line ))
//        {
//            istringstream line_stream( line );
//            getline(line_stream, ID, ':');
//            if (ID == "VmPeak"){
//                line_stream >> amnt;
//                line_stream >> unit;
//                return make_tuple(amnt, unit);
//            }
//        }
//    }
//    file_stream.close();
//    return make_tuple(0, "?");
//}

void MainProgram::test_get_functions(BeaconID id)
{
    ds_.get_name(id);
    ds_.get_coordinates(id);
    ds_.get_color(id);
}

MainProgram::CmdResult MainProgram::cmd_add_beacon(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    BeaconID id = *begin++;
    string name = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    string rstr = *begin++;
    string gstr = *begin++;
    string bstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

//    string heightstr = *begin++;
//    assert( begin == end && "Impossible number of parameters!");
//    int height;
//    if (!heightstr.empty())
//    {
//        height = convert_string_to<int>(heightstr);
//    }
//    else
//    {
//        height = -1; // REPLACE WITH HEIGTH FROM MAP!
//    }

    Coord xy = {convert_string_to<int>(xstr), convert_string_to<int>(ystr)};
    int r = convert_string_to<int>(rstr);
    int g = convert_string_to<int>(gstr);
    int b = convert_string_to<int>(bstr);

    bool success = ds_.add_beacon(id, name, xy, {r, g, b});

    view_dirty = true;
    return {ResultType::IDLIST, MainProgram::CmdResultIDs{success ? id: NO_ID}};
}

MainProgram::CmdResult MainProgram::cmd_change_name(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID id = *begin++;
    string newname = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool success = ds_.change_beacon_name(id, newname);

    view_dirty = true;
    return {ResultType::IDLIST, MainProgram::CmdResultIDs{success ? id : NO_ID}};
}

MainProgram::CmdResult MainProgram::cmd_change_color(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID id = *begin++;
    string rstr = *begin++;
    string gstr = *begin++;
    string bstr = *begin++;

    assert( begin == end && "Impossible number of parameters!");

    int r = convert_string_to<int>(rstr);
    int g = convert_string_to<int>(gstr);
    int b = convert_string_to<int>(bstr);

    bool success = ds_.change_beacon_color(id, {r, g, b});

    view_dirty = true;
    return {ResultType::IDLIST, MainProgram::CmdResultIDs{success ? id : NO_ID}};
}

void MainProgram::test_change_name()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto newname = n_to_name(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.change_beacon_name(id, newname);
        test_get_functions(id);
    }
}

void MainProgram::test_change_color()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto r = random(0, 255);
        auto g = random(0, 255);
        auto b = random(0, 255);
        ds_.change_beacon_color(id, {r, g, b});
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_add_lightbeam(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID sourceid = *begin++;
    BeaconID targetid = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool ok = ds_.add_lightbeam(sourceid, targetid);
    if (ok)
    {
        auto sourcename = ds_.get_name(sourceid);
        auto targetname = ds_.get_name(targetid);
        output << "Added lightbeam: " << sourcename << " -> " << targetname << endl;
    }
    else
    {
        output << "Adding lightbeam failed!" << endl;
    }

    view_dirty = true;
    return {};
}

MainProgram::CmdResult MainProgram::cmd_add_fibre(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string x1str = *begin++;
    string y1str = *begin++;
    string x2str = *begin++;
    string y2str = *begin++;
    string coststr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord xy1 = {convert_string_to<int>(x1str), convert_string_to<int>(y1str)};
    Coord xy2 = {convert_string_to<int>(x2str), convert_string_to<int>(y2str)};
    Cost cost = convert_string_to<int>(coststr);
    bool ok = ds_.add_fibre(xy1, xy2, cost);
    if (ok)
    {
        output << "Added fibre: (" << xy1.x << "," << xy1.y << ") <-> (" << xy2.x << "," << xy2.y << "), cost " << cost << endl;
    }
    else
    {
        output << "Adding fibre failed!" << endl;
    }

    view_dirty = true;
    return {};
}

MainProgram::CmdResult MainProgram::cmd_remove_fibre(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string x1str = *begin++;
    string y1str = *begin++;
    string x2str = *begin++;
    string y2str = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord xy1 = {convert_string_to<int>(x1str), convert_string_to<int>(y1str)};
    Coord xy2 = {convert_string_to<int>(x2str), convert_string_to<int>(y2str)};
    bool ok = ds_.remove_fibre(xy1, xy2);
    if (ok)
    {
        output << "Removed fibre: (" << xy1.x << "," << xy1.y << ") <-> (" << xy2.x << "," << xy2.y << ")" << endl;
    }
    else
    {
        output << "Removing fibre failed!" << endl;
    }

    view_dirty = true;
    return {};
}

void MainProgram::test_remove_fibre()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id1 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto id2 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.remove_fibre(ds_.get_coordinates(id1), ds_.get_coordinates(id2));
        test_get_functions(id1);
        test_get_functions(id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_path_outbeam(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.path_outbeam(id);
    if (result.empty()) { return {ResultType::HIERARCHY, MainProgram::CmdResultIDs{NO_ID}}; }
    else { return {ResultType::HIERARCHY, result}; }
}

void MainProgram::test_path_outbeam()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.path_outbeam(id);
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_path_inbeam_longest(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.path_inbeam_longest(id);
    if (result.empty()) { return {ResultType::HIERARCHY, MainProgram::CmdResultIDs{NO_ID}}; }
    else { return {ResultType::HIERARCHY, result}; }
}

void MainProgram::test_path_inbeam_longest()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.path_inbeam_longest(id);
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_total_color(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    BeaconID id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.total_color(id);
    auto name = ds_.get_name(id);
    output << "Total color of " << name << ": (" << result.r << "," << result.g << "," << result.b << ")" << endl;

    return {};
}

void MainProgram::test_total_color()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.total_color(id);
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_remove_beacon(ostream& output, MatchIter begin, MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto name = ds_.get_name(id);
    bool success = ds_.remove_beacon(id);
    if (success)
    {
        output << name << " removed." << endl;
        view_dirty = true;
        return {};
    }
    else
    {
        return {ResultType::IDLIST, MainProgram::CmdResultIDs{NO_ID}};
    }
}

void MainProgram::test_remove_beacon()
{
    // Choose random number to remove
    if (random_beacons_added_ > 0) // Don't remove if there's nothing to remove
    {
        auto name = n_to_name(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.remove_beacon(name);
    }
}

void MainProgram::add_random_beacons(unsigned int size, Coord min, Coord max)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        string name = n_to_name(random_beacons_added_);
        BeaconID id = n_to_id(random_beacons_added_);

        int x = random<int>(min.x, max.x);
        int y = random<int>(min.y, max.y);
        int r = random<int>(1, 255);
        int g = random<int>(1, 255);
        int b = random<int>(1, 255);

        ds_.add_beacon(id, name, {x, y}, {r, g, b});

        // Add random target beacon whose number is smaller, with 80 % probability
        if (random_beacons_added_ > 0 && random(0, 100) < 80)
        {
            BeaconID taxerid = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
            ds_.add_lightbeam(id, taxerid);
        }

        ++random_beacons_added_;
    }
}

MainProgram::CmdResult MainProgram::cmd_random_add(ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    string minxstr = *begin++;
    string minystr = *begin++;
    string maxxstr = *begin++;
    string maxystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    Coord def_min{1, 1};
    Coord def_max{100, 100};
    Coord min = def_min;
    Coord max = def_max;
    if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() && !maxystr.empty())
    {
        min.x = convert_string_to<unsigned int>(minxstr);
        min.y = convert_string_to<unsigned int>(minystr);
        max.x = convert_string_to<unsigned int>(maxxstr);
        max.y = convert_string_to<unsigned int>(maxystr);
    }
    else
    {
        auto beacons = ds_.all_beacons();
        if (!beacons.empty())
        {
            // Find out bounding box
            min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
            for (auto const& beacon : beacons)
            {
                auto [x,y] = ds_.get_coordinates(beacon);
                if (x < min.x) { min.x = x; }
                if (y < min.y) { min.y = y; }
                if (x > max.x) { max.x = x; }
                if (y > max.y) { max.y = y; }
            }
        }
    }

    if (min == max)
    {
        min = def_min;
        max = def_max;
    }

    add_random_beacons(size, min, max);

    output << "Added: " << size << " beacons." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_add()
{
    add_random_beacons(1);
}

MainProgram::CmdResult MainProgram::cmd_randseed(ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, output, PromptStyle::NORMAL);
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = max_element(actual_lines.cbegin(), actual_lines.cend(),
                                               [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_beacon_count(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of beacons: " << ds_.beacon_count() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_all_beacons(ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto beacons = ds_.all_beacons();
    if (beacons.empty())
    {
        output << "No beacons!" << endl;
    }

    return {ResultType::IDLIST, beacons};
}

void MainProgram::test_all_beacons()
{
    ds_.all_beacons();
}

MainProgram::CmdResult MainProgram::cmd_all_xpoints(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto xpoints = ds_.all_xpoints();
    if (xpoints.empty())
    {
        output << "No xpoints!" << endl;
    }

    return {ResultType::COORDLIST, xpoints};
}

void MainProgram::test_all_xpoints()
{
    ds_.all_xpoints();
}

MainProgram::CmdResult MainProgram::cmd_all_fibres(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto fibres = ds_.all_fibres();
    if (fibres.empty())
    {
        output << "No fibres!" << endl;
    }
    else
    {
        for (auto& [xy1, xy2] : fibres)
        {
            output << "(" << xy1.x << "," << xy1.y << ") -> (" << xy2.x << "," << xy2.y << ")" << endl;
        }
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_lightsources(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string id = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto sources = ds_.get_lightsources(id);
    if (sources.empty())
    {
        output << "No lightsources!" << endl;
    }

    return {ResultType::IDLIST, sources};
}

void MainProgram::test_lightsources()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.get_lightsources(id);
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_fibres(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord xy = {convert_string_to<int>(xstr), convert_string_to<int>(ystr)};
    auto fibres = ds_.get_fibres_from(xy);
    if (fibres.empty())
    {
        output << "No fibres!" << endl;
    }

    return {ResultType::FIBRELIST, fibres};
}

void MainProgram::test_fibres()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.get_fibres_from(ds_.get_coordinates(id));
        test_get_functions(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_random_fibres(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string sizestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    if (ds_.beacon_count() < 2)
    {
        output << "Not enough beacons to add fibres!" << std::endl;
        return {};
    }

    unsigned int random_fibres = convert_string_to<unsigned int>(sizestr);

    add_random_fibres(output, random_fibres);

    output << "Added at most " << random_fibres << " random fibres." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_fibres()
{
    if (random_beacons_added_ > 0) // Don't do anything if there's no beacons
    {
        for (auto i=0; i<10; ++i)
        {
            auto id1 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
            auto id2 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
            Cost cost = random(0, 100);
            ds_.add_fibre(ds_.get_coordinates(id1), ds_.get_coordinates(id2), cost);
        }
    }
}

MainProgram::CmdResult MainProgram::cmd_random_labyrinth(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    string extrastr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int xsize = convert_string_to<int>(xstr);
    int ysize = convert_string_to<int>(ystr);
    int extra = convert_string_to<int>(extrastr);

    create_fibre_labyrinth(output, xsize, ysize, extra);

    output << "Added fibre labyrinth with at most " << extra << " extra routes." << endl;

    view_dirty = true;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_clear_beacons(ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear_beacons();
    init_primes();

    output << "Cleared all beacons" << endl;

    view_dirty = true;

    return {};
}

std::string MainProgram::print_beacon(BeaconID id, ostream& output)
{
    if (id != NO_ID)
    {
        auto name = ds_.get_name(id);
        auto [x,y] = ds_.get_coordinates(id);
        if (!name.empty())
        {
            Color color = ds_.get_color(id);
            int brightness = 3*color.r + 6*color.g + color.b;
            output << name << ": pos=(" << x << "," << y << "), color=(" << color.r << "," << color.g << "," << color.b << "):" << brightness
                   << ", id=" << id << endl;
        }
        else
        {
            output << "*" << ": pos=(" << x << "," << y << "), id=" << id << endl;
        }

        ostringstream retstream;
        retstream << id;
        return retstream.str();
    }
    else
    {
        output << "-- unknown beacon --" << endl;
        return "";
    }
}

std::string MainProgram::print_coord(Coord coord, std::ostream& output)
{
    if (coord != NO_COORD)
    {
        output << "(" << coord.x << "," << coord.y << ")" << endl;
        ostringstream retstream;
        retstream << "(" << coord.x << "," << coord.y << ")";
        return retstream.str();
    }
    else
    {
        output << "-- unknown coordinates --" << endl;
        return "";
    }
}

MainProgram::CmdResult MainProgram::cmd_find_beacons(ostream& /*output*/, MatchIter begin, MatchIter end)
{
    string name = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.find_beacons(name);
    if (result.empty()) { result = {NO_ID}; }

    return {ResultType::IDLIST, result};
}

void MainProgram::test_find_beacons()
{
    // Choose random number to remove
    if (random_beacons_added_ > 0) // Don't find if there's nothing to find
    {
        auto name = n_to_name(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.find_beacons(name);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_any(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int fromx = convert_string_to<int>(fromxstr);
    int fromy = convert_string_to<int>(fromystr);
    int tox = convert_string_to<int>(toxstr);
    int toy = convert_string_to<int>(toystr);

    auto result = ds_.route_any({fromx, fromy}, {tox, toy});

    if (result.empty())
    {
        output << "No path found!" << endl;
    }

    return {ResultType::PATH, result};
}

void MainProgram::test_route_any()
{
    if (random_beacons_added_ > 0)
    {
        // Choose two random beacons
        auto id1 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto id2 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.route_any(ds_.get_coordinates(id1), ds_.get_coordinates(id2));
    }
}

MainProgram::CmdResult MainProgram::cmd_route_fastest(ostream& output, MatchIter begin, MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int fromx = convert_string_to<int>(fromxstr);
    int fromy = convert_string_to<int>(fromystr);
    int tox = convert_string_to<int>(toxstr);
    int toy = convert_string_to<int>(toystr);

    auto result = ds_.route_fastest({fromx, fromy}, {tox, toy});

    if (result.empty())
    {
        output << "No path found!" << endl;
    }

    return {ResultType::PATH, result};
}

void MainProgram::test_route_fastest()
{
    if (random_beacons_added_ > 0)
    {
        // Choose two random beacons
        auto id1 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto id2 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.route_fastest(ds_.get_coordinates(id1), ds_.get_coordinates(id2));
    }
}

MainProgram::CmdResult MainProgram::cmd_route_least_xpoints(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int fromx = convert_string_to<int>(fromxstr);
    int fromy = convert_string_to<int>(fromystr);
    int tox = convert_string_to<int>(toxstr);
    int toy = convert_string_to<int>(toystr);

    auto result = ds_.route_least_xpoints({fromx, fromy}, {tox, toy});

    if (result.empty())
    {
        output << "No path found!" << endl;
    }

    return {ResultType::PATH, result};
}

void MainProgram::test_route_least_xpoints()
{
    if (random_beacons_added_ > 0)
    {
        // Choose two random towns
        auto id1 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        auto id2 = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.route_least_xpoints(ds_.get_coordinates(id1), ds_.get_coordinates(id2));
    }
}

MainProgram::CmdResult MainProgram::cmd_route_fibre_cycle(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    int fromx = convert_string_to<int>(fromxstr);
    int fromy = convert_string_to<int>(fromystr);

    auto result = ds_.route_fibre_cycle({fromx, fromy});

    if (result.empty())
    {
        output << "No fibre cycles found." << std::endl;
    }

    return {ResultType::CYCLE, result};
}

void MainProgram::test_route_fibre_cycle()
{
    if (random_beacons_added_ > 0)
    {
        // Choose random town
        auto id = n_to_id(random<decltype(random_beacons_added_)>(0, random_beacons_added_));
        ds_.route_fibre_cycle(ds_.get_coordinates(id));
    }
}

MainProgram::CmdResult MainProgram::cmd_trim_fibre_network(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    Cost total_cost;
    total_cost = ds_.trim_fibre_network();

    output << "The remaining fibre network has total cost of " << total_cost << std::endl;

    view_dirty = true;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_clear_fibres(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    ds_.clear_fibres();
    output << "All fibres removed." << std::endl;

    return {};
}

void MainProgram::test_trim_fibre_network()
{
    ds_.trim_fibre_network();
}

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
{
    {"add_beacon", "ID Name (x,y) (r,g,b)",
     "([a-zA-Z0-9]+)[[:space:]]+([a-zA-Z]+)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+),[[:space:]]*([0-9]+)\\)",
     &MainProgram::cmd_add_beacon, nullptr },
    {"random_add", "number_of_beacons_to_add  (minx,miny) (maxx,maxy) (coordinates optional)",
     "([0-9]+)(?:[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\))?",
     &MainProgram::cmd_random_add, &MainProgram::test_random_add },
    {"random_fibres", "max_number_of_fibres_to_add", "([0-9]+)",
     &MainProgram::cmd_random_fibres, &MainProgram::test_random_fibres },
    {"random_labyrinth", "xsize ysize extra_routes",
     "([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)", &MainProgram::cmd_random_labyrinth, nullptr },
    {"all_beacons", "", "", &MainProgram::cmd_all_beacons, &MainProgram::test_all_beacons },
    {"all_xpoints", "", "", &MainProgram::cmd_all_xpoints, &MainProgram::test_all_xpoints },
    {"all_fibres", "", "", &MainProgram::cmd_all_fibres, nullptr },
    {"beacon_count", "", "", &MainProgram::cmd_beacon_count, nullptr },
    {"clear_beacons", "", "", &MainProgram::cmd_clear_beacons, nullptr },
    {"sort_alpha", "", "", &MainProgram::NoParBeaconListCmd<&Datastructures::beacons_alphabetically>, &MainProgram::NoParListTestCmd<&Datastructures::beacons_alphabetically> },
    {"sort_brightness", "", "", &MainProgram::NoParBeaconListCmd<&Datastructures::beacons_brightness_increasing>, &MainProgram::NoParListTestCmd<&Datastructures::beacons_brightness_increasing> },
    {"min_brightness", "", "", &MainProgram::NoParBeaconCmd<&Datastructures::min_brightness>, &MainProgram::NoParBeaconTestCmd<&Datastructures::min_brightness> },
    {"max_brightness", "", "", &MainProgram::NoParBeaconCmd<&Datastructures::max_brightness>, &MainProgram::NoParBeaconTestCmd<&Datastructures::max_brightness> },
    {"remove_beacon", "ID", "([A-Za-z0-9]+)", &MainProgram::cmd_remove_beacon, &MainProgram::test_remove_beacon },
    {"find_beacons", "name", "([a-zA-Z]+)", &MainProgram::cmd_find_beacons, &MainProgram::test_find_beacons },
    {"change_name", "ID newname", "([a-zA-Z0-9]+)[[:space:]]+([a-zA-Z]+)", &MainProgram::cmd_change_name, &MainProgram::test_change_name },
    {"change_color", "ID (r,g,b)",
     "([a-zA-Z0-9]+)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+),[[:space:]]*([0-9]+)\\)",
     &MainProgram::cmd_change_color, &MainProgram::test_change_color },
    {"add_lightbeam", "SourceID TargetID",
     "([a-zA-Z0-9]+)[[:space:]]+([a-zA-Z0-9]+)", &MainProgram::cmd_add_lightbeam, nullptr },
    {"lightsources", "BeaconID", "([a-zA-Z0-9]+)", &MainProgram::cmd_lightsources, &MainProgram::test_lightsources },
    {"add_fibre", "(x1,y1) (x2,y2) cost",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+([0-9]+)", &MainProgram::cmd_add_fibre, nullptr },
    {"remove_fibre", "(x1,y1) (x2,y2)",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_remove_fibre, &MainProgram::test_remove_fibre },
    {"fibres", "(x,y)", "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_fibres, &MainProgram::test_fibres },
    {"clear_fibres", "", "", &MainProgram::cmd_clear_fibres, nullptr },
    {"path_outbeam", "ID", "([A-Za-z0-9]+)", &MainProgram::cmd_path_outbeam, &MainProgram::test_path_outbeam },
    {"path_inbeam_longest", "ID", "([A-Za-z0-9]+)", &MainProgram::cmd_path_inbeam_longest, &MainProgram::test_path_inbeam_longest },
    {"total_color", "ID", "([A-Za-z0-9]+)", &MainProgram::cmd_total_color, &MainProgram::test_total_color },
    {"route_any", "(x1,y1) (x2,y2)",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_route_any, &MainProgram::test_route_any },
    {"route_fastest", "(x1,y1) (x2,y2)",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_route_fastest, &MainProgram::test_route_fastest },
    {"route_least_xpoints", "(x1,y1) (x2,y2)",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)[[:space:]]+\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_route_least_xpoints, &MainProgram::test_route_least_xpoints },
    {"route_fibre_cycle", "(x1,y1)",
     "\\(([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)\\)", &MainProgram::cmd_route_fibre_cycle, &MainProgram::test_route_fibre_cycle },
    {"trim_fibre_network", "", "", &MainProgram::cmd_trim_fibre_network, &MainProgram::test_trim_fibre_network },
    {"quit", "", "", nullptr, nullptr },
    {"help", "", "", &MainProgram::help_command, nullptr },
    {"read", "\"in-filename\"",
     "\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_read, nullptr },
    {"testread", "\"in-filename\" \"out-filename\"",
     "\"([-a-zA-Z0-9 ./:_]+)\"[[:space:]]+\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
//    {"perftest", "",
//     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+(?:;[0-9]+)*)", &cmd_perftest, nullptr },
    {"perftest", "cmd1/all/compulsory[;cmd2;cmd3...] timeout repeat_count n1[;n2;n3...] (parts in [] are optional)",
     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+)[[:space:]]+([0-9]+(?:;[0-9]+)*)", &MainProgram::cmd_perftest, nullptr },
    {"stopwatch", "on/off/next (one of these)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
    {"random_seed", "new-random-seed-integer", "([0-9]+)", &MainProgram::cmd_randseed, nullptr },
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
};

MainProgram::CmdResult MainProgram::help_command(ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    vector<string> optional_cmds({"remove_beacon", "path_inbeam_longest", "total_color"});
    vector<string> nondefault_cmds({"all_beacons", "remove_beacon", "find_beacons"});

    string commandstr = *begin++;
    unsigned int timeout = convert_string_to<unsigned int>(*begin++);
    unsigned int repeat_count = convert_string_to<unsigned int>(*begin++);
//    unsigned int friend_count = convert_string_to<unsigned int>(*begin++);
    string sizes = *begin++;
    assert(begin == end && "Invalid number of parameters");

    vector<string> testcmds;
    if (commandstr != "all" && commandstr != "compulsory")
    {
        regex commands_regex("([0-9a-zA-Z_]+);?");
        smatch scmd;
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        for ( ; regex_search(cbeg, cend, scmd, commands_regex); cbeg = scmd.suffix().first)
        {
            testcmds.push_back(scmd[1]);
        }
    }

    vector<unsigned int> init_ns;
    regex sizes_regex("([0-9]+);?");
    smatch size;
    auto sbeg = sizes.cbegin();
    auto send = sizes.cend();
    for ( ; regex_search(sbeg, send, size, sizes_regex); sbeg = size.suffix().first)
    {
        init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }

    output << "Timeout for each N is " << timeout << " sec. " << endl;
//    output << "Add 0.." << friend_count << " friends for every employee." << endl;
    output << "For each N perform " << repeat_count << " random command(s) from:" << endl;

    // Initialize test functions
    vector<void(MainProgram::*)()> testfuncs;
    if (testcmds.empty())
    { // Add all commands
        for (auto& i : cmds_)
        {
            if (i.testfunc)
            {
                if (find(nondefault_cmds.begin(), nondefault_cmds.end(), i.cmd) == nondefault_cmds.end() &&
                    (commandstr == "all" || find(optional_cmds.begin(), optional_cmds.end(), i.cmd) == optional_cmds.end()))
                {
                    output << i.cmd << " ";
                    testfuncs.push_back(i.testfunc);
                }
            }
        }
    }
    else
    {
        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto& cmd){ return cmd.cmd == i; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i << " ";
                testfuncs.push_back(pos->testfunc);
            }
            else
            {
                output << "(cannot test " << i << ") ";
            }
        }
    }
    output << endl << endl;

    if (testfuncs.empty())
    {
        output << "No commands to test!" << endl;
        return {};
    }

    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)"  << " , " << setw(12) << "total (sec)" << endl;
    flush_output(output);

    auto stop = false;
    for (unsigned int n : init_ns)
    {
        if (stop) { break; }

        output << setw(7) << n << " , " << flush;

        ds_.clear_beacons();
        ds_.clear_fibres();
        init_primes();

        Stopwatch stopwatch;
        stopwatch.start();

        // Add random beacons (+ light sources)
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            add_random_beacons(1000);

            stopwatch.stop();
            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
            stopwatch.start();
        }
        if (stop) { break; }

        add_random_beacons(n % 1000);

        // Add random fibres
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            for (unsigned int j=0; j<1000/15; ++j)
            {
                test_random_fibres();
            }

            stopwatch.stop();
            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
            stopwatch.start();
        }
        if (stop) { break; }

        for (unsigned int j=0; j< (n % 1000)/15; ++j)
        {
            test_random_fibres();
        }

        auto addsec = stopwatch.elapsed();
        output << setw(12) << addsec << " , " << flush;
        if (addsec >= timeout)
        {
            output << "Timeout!" << endl;
            stop = true;
            break;
        }

        for (unsigned int repeat = 0; repeat < repeat_count; ++repeat)
        {
            auto cmdpos = random(testfuncs.begin(), testfuncs.end());

            (this->**cmdpos)();

            if (repeat % 10 == 0)
            {
                stopwatch.stop();
                if (stopwatch.elapsed() >= timeout)
                {
                    output << "Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
                stopwatch.start();
            }
        }
        if (stop) { break; }

        stopwatch.stop();
        auto totalsec = stopwatch.elapsed();
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;

//        unsigned long int maxmem;
//        string unit;
//        tie(maxmem, unit) = mempeak();
//        maxmem -=  startmem;
//        if (maxmem != 0)
//        {
//            output << ", memory " << maxmem << " " << unit;
//        }
        output << endl;
        flush_output(output);
    }

    ds_.clear_beacons();
    ds_.clear_fibres();
    init_primes();

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

bool MainProgram::command_parse_line(string inputline, ostream& output)
{
//    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
//    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;
    }
    cmds_regex_str += ")(?:[[:space:]]*$|[[:space:]]+(.*))";
    regex cmds_regex(cmds_regex_str);

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        string params_regex_str = pos->param_regex_str+"[[:space:]]*";
        smatch match;
        bool matched = regex_match(params, match, regex(params_regex_str));
        if (matched)
        {
            if (pos->func)
            {
                assert(!match.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

               TestStatus initial_status = test_status_;
               test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                auto result = (this->*(pos->func))(output, ++(match.begin()), match.end());

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                switch (result.first)
                {
                    case ResultType::IDLIST:
                    {
                        auto& res = std::get<CmdResultIDs>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front() == NO_ID)
                            {
                                output << NO_NAME << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (BeaconID id : res)
                                {
                                    ++num;
                                    if (res.size() > 1) { output << num << ". "; }
                                    print_beacon(id, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::COORDLIST:
                    {
                        auto& res = std::get<CmdResultCoords>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front() == NO_COORD)
                            {
                                output << "Unknown coordinate (NO_COORD)" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (Coord coord: res)
                                {
                                    ++num;
                                    if (res.size() > 1) { output << num << ". "; }
                                    print_coord(coord, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::HIERARCHY:
                    {
                        auto& res = std::get<CmdResultIDs>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front() == NO_ID)
                            {
                                output << NO_NAME << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (BeaconID id : res)
                                {
                                    if (res.size() > 1)
                                    {
                                        output << num << ". ";
                                        if (num > 0) { output << "-> "; }
                                        else { output << "   "; }
                                    }
                                    ++num;
                                    print_beacon(id, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::FIBRELIST:
                    {
                        auto& res = std::get<CmdResultFibres>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front().first == NO_COORD)
                            {
                                output << "No coordinates" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 1;
                                for (auto& [coord, cost] : res)
                                {
                                    output << num << ". ";
                                    ++num;
                                    output << "(" << coord.x << "," << coord.y << ") : " << cost << std::endl;
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::PATH:
                    {
                        auto& res = std::get<CmdResultFibres>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front().first == NO_COORD)
                            {
                                output << "No coordinates" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (auto& [coord, cost] : res)
                                {
                                    output << num << ". ";
                                    if (num > 0) { output << "-> "; }
                                    else { output << "   "; }
                                    ++num;
                                    output << "(" << coord.x << "," << coord.y << ") : " << cost << std::endl;
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::CYCLE:
                    {
                        auto& res = std::get<CmdResultCoords>(result.second);
                        if (!res.empty())
                        {
                            if (res.size() == 1 && res.front() == NO_COORD)
                            {
                                output << "Unknown coordinate (NO_COORD)" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                if (res.size() < 2)
                                {
                                    output << "Too small path for cycle!";
                                }
                                else
                                {
                                    auto cycbeg = std::find(res.begin(), res.end()-1, res.back());
                                    if (cycbeg == res.end())
                                    {
                                        output << "No cycle found in path!";
                                    }
                                    else
                                    {
                                        // Swap cycle so that it starts with smaller id
                                        if (((cycbeg+1) < (res.end()-2)) && (*(res.end()-2) < *(cycbeg+1)))
                                        {
                                            std::reverse(cycbeg+1, res.end()-1);
                                        }
                                        for (auto i = cycbeg; i != res.end(); ++i)
                                        {
                                            Coord xy = *i;
                                            output << num << ". ";
                                            if (num > 0) { output << "-> "; }
                                            else { output << "   "; }
                                            ++num;
                                            output << "(" << xy.x << "," << xy.y << ")" << std::endl;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    default:
                    {
                        assert(false && "Unsupported result type!");
                    }
                }

                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec" << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
//        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2 && args[1] != "--console")
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    return EXIT_SUCCESS;
}

void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_beacons_added_ = 0;
}

string MainProgram::n_to_name(unsigned long n)
{
    unsigned long int hash = prime1_*n + prime2_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}

std::string MainProgram::n_to_id(unsigned long n)
{
    unsigned long int hash = prime2_*n + prime1_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}

void MainProgram::create_fibre_labyrinth(std::ostream& output, int xsize, int ysize, int extrafibres)
{
    // Clear existing fibres
    ds_.clear_fibres();

    // Vector to collect all connections
    vector<XpointInfo> xpoints(xsize*ysize);

    // Add random fibre network
    // false = fibre, true = no connection
    vector<bool> fibres(xsize*ysize*ENDDIR);

    vector<long int> tset(xsize*ysize, -1);
    auto fibrenum = fibres.size();
    while (fibrenum > 0)
    {
        // Find a random fibre
        auto fibreiter =  random(fibres.begin(), fibres.end());
        fibreiter = find(fibreiter, fibres.end(), false); // Find next fibre
        if (fibreiter == fibres.end())
        {
            fibreiter = find(fibres.begin(), fibres.end(), false); // Find from beginning
        }
        assert(fibreiter != fibres.end() && "Run out of fibres!");
        *fibreiter = true; // Remove fibre

        // Calculate fibre coordinates and direction
        auto fibreidx = fibreiter - fibres.begin();
        auto [x1, rest] = div(fibreidx, static_cast<decltype(fibreidx)>(ysize*ENDDIR));
        auto [y1, dir] = div(rest, static_cast<decltype(fibreidx)>(ENDDIR));

        // Find the next coordinate
        auto [x2, y2] = move_to_dir({static_cast<int>(x1), static_cast<int>(y1)}, static_cast<Dir>(dir));
        if (x2 < 0 || y2 < 0 || x2 >= xsize || y2 >= ysize) { continue; } // If out of labyrinth area

        auto i1 = x1*ysize + y1;
        auto i2 = x2*ysize + y2;

        auto s1 = i1;
        while (tset[s1] >= 0) { s1 = tset[s1]; }
        auto s2 = i2;
        while (tset[s2] >= 0) { s2 = tset[s2]; }
        if (s1 == s2) { continue; } // Already connected
        tset[s2] += tset[s1];
        tset[s1] = s2;

        xpoints[x1*ysize + y1].dirs[dir] = true;
        xpoints[x2*ysize + y2].dirs[invert_dir[dir]] = true;
        if (tset[s2] == -static_cast<long int>(xsize*ysize)) { break; }
    }

    // Add extra connections
    for (int i=0; i<extrafibres; ++i)
    {
        auto x1 = random(0, xsize);
        auto y1 = random(0, ysize);
        auto dir = random(0, static_cast<int>(ENDDIR));
        auto [x2, y2] = move_to_dir({x1, y1}, static_cast<Dir>(dir));
        if (x2 < 0 || x2 >= xsize || y2 < 0 || y2 >= ysize) { continue; }
        xpoints[x1*ysize + y1].dirs[dir] = true; // New fibre
        xpoints[x2*ysize + y2].dirs[invert_dir[dir]] = true; // And also in other direction
    }

    // Add fibres
    Coord start{random(0, xsize), random(0, ysize)};
    vector<Coord> beaconxys;
    beaconxys.reserve(ds_.beacon_count());
    for (auto id : ds_.all_beacons()) // Make sure all beacons have a corresponding xpoint
    {
        auto [x, y] = ds_.get_coordinates(id);
        auto x2 = x/2;
        auto y2 = y/2;
        if (y % 2 == 0 && x % 2 == y2 % 1)
        {
            beaconxys.push_back({x2, y2});
        }
    }
    sort(beaconxys.begin(), beaconxys.end());
    add_labyrinth_fibres(output, {xsize, ysize}, start, start, ENDDIR, 0, xpoints, beaconxys);
}

Coord MainProgram::move_to_dir(Coord coord, MainProgram::Dir dir)
{
    auto [x2, y2] = coord;
    switch (dir)
    {
        case WEST: { --x2; break; }
        case EAST: { ++x2; break; }
        case NORTHWEST: { ++y2; x2 -= (y2 % 2); break; }
        case NORTHEAST: { x2 += (y2 % 2); ++y2; break; }
        case SOUTHWEST: { --y2; x2 -= (y2 % 2); break; }
        case SOUTHEAST: { x2 += (y2 % 2); --y2; break; }
        default: { assert(!"END as move direction!"); }
    }
    return {x2, y2};
}

void MainProgram::add_labyrinth_fibres(std::ostream& output, Coord size, Coord pos, Coord from, Dir fromdir, Cost fromcost, vector<XpointInfo>& xpoints, vector<Coord> const& beaconxys)
{
    Cost const UNITCOST = 2;
    auto& xpointinfo = xpoints[pos.x*size.y + pos.y];
    if (fromdir!=ENDDIR && xpointinfo.dirs[fromdir] && xpointinfo.dirs.count() == 2 &&
            !binary_search(beaconxys.begin(), beaconxys.end(), pos)) // Only connection is to the same direction, and we are not at a beacon
    {
        xpointinfo.visited = true; // Visited now
        add_labyrinth_fibres(output, size, move_to_dir(pos, fromdir), from, fromdir, fromcost+UNITCOST, xpoints, beaconxys); // Continue in the same direction
    }
    else
    {
        Coord xypos{2*pos.x + (pos.y % 2), 2*pos.y};
        Coord xyfrom{2*from.x + (from.y % 2), 2*from.y};
        if (xyfrom < xypos)
        {
            ds_.add_fibre(xyfrom, xypos, fromcost+UNITCOST); // Add fibre to here
            output << "add_fibre (" << xyfrom.x << "," << xyfrom.y << ") (" << xypos.x << "," << xypos.y << ") " << fromcost+UNITCOST << endl;
        }
        if (xpointinfo.visited) { return; } // Already visited, no need to scan further
        xpointinfo.visited = true; // Visited now
        for (int dir=FIRSTDIR; dir!=ENDDIR; ++dir)
        {
            if (xpointinfo.dirs[dir])
            {
                auto pos2 = move_to_dir(pos, static_cast<Dir>(dir));
                add_labyrinth_fibres(output, size, pos2, pos, static_cast<Dir>(dir), 0, xpoints, beaconxys);
            }
        }
    }
}

void MainProgram::add_random_fibres(std::ostream& output, unsigned int random_fibres)
{
    vector<pair<Coord, Coord>> addedfibres;
    auto beacons = ds_.all_beacons();
    sort(beacons.begin(), beacons.end()); // Sort beacon IDs to get deterministic results

    // Add given number of totally random fibres
    for ( ; random_fibres != 0; --random_fibres)
    {
        auto i1 = random(beacons.begin(), beacons.end());
        auto i2 = random(beacons.begin(), beacons.end());
        auto cost = random(0, 100);
        if (i1 != i2)
        {
            Coord p1 = ds_.get_coordinates(*i1);
            Coord p2 = ds_.get_coordinates(*i2);
            bool intersects = false;
            for (auto const& fibre : addedfibres)
            {
                if (doIntersect(p1, p2, fibre.first, fibre.second)) { intersects = true; break; }
            }
            if (intersects) { continue; }
            auto xyfrom = ds_.get_coordinates(*i1);
            auto xyto = ds_.get_coordinates(*i2);
            ds_.add_fibre(xyfrom, xyto, cost);
            output << "add_fibre (" << xyfrom.x << "," << xyfrom.y << ") (" << xyto.x << "," << xyto.y << ") " << cost << endl;
            addedfibres.push_back({p1, p2});
        }
    }
}

// The functions below are taken and modified from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// point q lies on line segment 'pr'
bool MainProgram::onSegment(Coord p, Coord q, Coord r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int MainProgram::orientation(Coord p, Coord q, Coord r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool MainProgram::doIntersect(Coord p1, Coord q1, Coord p2, Coord q2)
{
    if ((p1 == p2 && q1 == q2) || (p1 == q2 && q1 == p2)) { return true; } // Same line
    if (p1 == p2 || p1 == q2 || q1 == p2 || q1 == q2) { return false; } // One same point, cannot intersect
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
