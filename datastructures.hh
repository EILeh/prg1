// Datastructures.hh


#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <cmath>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

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
    Distance distance1 = sqrt(pow(c1.x, 2) + pow(c1.y, 2));
    Distance distance2 = sqrt(pow(c2.x, 2) + pow(c2.y, 2));
    if (distance1 < distance2) {return true;}
    if (distance1 > distance2) {return false;}
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: .size() is O(1)
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: .clear() is linear so O(n)
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: returns a copy of a complete vector
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: everything is O(1)
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and rest are O(1)
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything is else O(1)
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is O(n) and rest are O(1)
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is O(n) and rest are O(1)
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and rest are O(1)
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything else is O(1)
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: emplace is O(n) and everything else is O(1)
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: returns a copy of a complete vector
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: everything is O(1)
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: everything is O(1)
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything else is O(1)
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything else is O(1)
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything else is O(1)
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find is O(n) and everything else is O(1)
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is O(n) and rest are O(1)
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: everything is O(1)
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: sort is O(nlog(n))
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: while loop is O(n)
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: in the worst case visits all children once
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: sort is O(nlog(n))
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is O(n)
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: two nested for loops which are O(n)
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is O(n)
    bool remove_publication(PublicationID publicationid);


private:

    // Struct for an affiliation
    struct Affiliation
    {
        Name name = NO_NAME;
        Coord coordinates = NO_COORD;
        // Vector for all of the publications that related to this affiliation
        // first in the pair is the publication and second is publication year.
        std::vector<std::pair<PublicationID, Year>> affiliatedPubs = {};
    };

    // Unordered map containing all the affiliations where the key is the
    // AffiliationID and the value is the corresponding Affiliation struct.
    std::unordered_map<AffiliationID, Affiliation> affiliations = {};

    // Struct for a publication
    struct Publication
    {
        Name heading = NO_NAME;
        Year publishYear = NO_YEAR;
        // Vector containing all of the ID's of the affiliations this publication
        // is related to.
        std::vector<AffiliationID> affiliationsOfPub = {};
        // Vector containing all of the publicationID's that reference this publication
        std::vector<PublicationID> referencesOfPub = {};
        // Vector containing all of the publications that reference this publication.
        // Same as above but both useful for different things.
        std::vector<std::pair<PublicationID, Publication*>> children = {};
        // Pair containing the parent of this publication (the publication that this
        // publication references).
        std::pair<PublicationID, Publication*> parent = {NO_PUBLICATION,nullptr};
    };

    // Unordered_map containing all of the publications where the key is the
    // PublicationID and the value is the corresponding Publication struct.
    std::unordered_map<PublicationID, Publication> publications = {};

    // Vector containing all of the publicationID's.
    std::vector<PublicationID> publicationVec = {};

    // Map for storing all of the affiliations in an alphabetical order
    // according to the name of the affiliation. Key is the name and
    // the value is the ID of the affiliation.
    std::map<Name, AffiliationID> affilAlphabetic = {};

    // Map for storing all of the affiliations in a distance
    // increasing order from the origin (0,0). Key is the
    // coordinates and the value is the ID of the affiliation.
    std::map<Coord, AffiliationID> affilDistIncr = {};

    // Vector containing all fot the affiliationID's.
    std::vector<AffiliationID> affilIDVec = {};

    // Vectir containing all of the affiliationID's
    // sorted by the name of the affiliation.
    std::vector<AffiliationID> affilIDVecAlph = {};
    // Bool value that is true when the affilIDVecAlph is sorted
    // and false when not.
    bool alphabeticallySorted = true;

    // Vectir containing all of the affiliationID's sorted
    // by the distance from origin of the affiliation.
    std::vector<AffiliationID> affilIDVecDist = {};
    // Bool value that is true when the affilIDVecDist is sorted
    // and false when not.
    bool distIncrSorted = true;

    // Helper recursive function for the get_all_references function.
    void get_all_references_recursive(const Publication& publication, std::vector<PublicationID>& references);
};

#endif // DATASTRUCTURES_HH
