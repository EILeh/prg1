// Datastructures.cc


#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
}

unsigned int Datastructures::get_affiliation_count()
{
    return affilIDVec.size();
}

void Datastructures::clear_all()
{
    // Clearing all of the containers in the .hh file
    affiliations.clear();
    publications.clear();
    publicationVec.clear();
    affilAlphabetic.clear();
    affilDistIncr.clear();
    affilIDVec.clear();
    affilIDVecAlph.clear();
    affilIDVecDist.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return affilIDVec;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    // Adding the affiliation to all of the corresponding containers
    auto insertion1 = affiliations.emplace(id, Affiliation{name, xy});
    affilAlphabetic.emplace(name, id);
    affilDistIncr.emplace(xy, id);
    affilIDVec.push_back(id);

    // Clearing the sorted vectors and setting the bool values as false
    affilIDVecAlph.clear();
    affilIDVecDist.clear();
    distIncrSorted = false;
    alphabeticallySorted = false;

    // Returns insertion1.second which is a bool value that is true if the emplacement worked
    return insertion1.second;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    // finding the id in the affiliations map
    auto it = affiliations.find(id);
    if (it != affiliations.end()) {
        // returning the name of the affiliation
        return it->second.name;
    }
    return NO_NAME;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    // finding the id in the affiliations map
    auto it = affiliations.find(id);
    if (it != affiliations.end()) {
        // returning the coordinates of the affiliation
        return it->second.coordinates;
    }
    return NO_COORD;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    // If the bool value is false, the vector is not sorted and is empty
    if (!alphabeticallySorted) {
	affilIDVecAlph.reserve(affiliations.size());
        // Adding all of the ID's from the affilAlphabetic into the vector
        for (const auto& part : affilAlphabetic) {
            affilIDVecAlph.push_back(part.second);
        }
        alphabeticallySorted = true;
    }

    return affilIDVecAlph;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    // If the bool value is false, the vector is not sorted and is empty
    if (!distIncrSorted) {
        // Adding all of the ID's from the affilAlphabetic into the vector
        for (const auto& part : affilDistIncr) {
            affilIDVecDist.push_back(part.second);
        }
        distIncrSorted = true;
    }

    return affilIDVecDist;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    // Finding the id in the affilDistIncr map
    auto it = affilDistIncr.find(xy);

    if (it != affilDistIncr.end()) {
        // Returning the id of the affiliation in the given coordinates
        return it->second;
    }
    return NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it1 = affiliations.find(id);
    if (it1 != affiliations.end()) {
        // Removing the affiliation from the affilDistIncr map
        auto it2 = affilDistIncr.find(it1->second.coordinates);
        affilDistIncr.erase(it2);
        // Adding the affiliation back with new coordinates
        affilDistIncr.emplace(newcoord ,id);
        it1->second.coordinates = newcoord;

        // Coordinate changed so the affilIDVecDist is no longer sorted
        affilIDVecDist.clear();
        distIncrSorted = false;
        return true;
    }
    return false;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliationsOfPub)
{
    auto insertion = publications.emplace(id, Publication{name, year, affiliationsOfPub});
    publicationVec.push_back(id);
    // Insertion second is a bool value that is true if the emplacement was successful
    return insertion.second;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    return publicationVec;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end()) {
        // Returning the heading of the publication
        return it->second.heading;
    }
    return NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end()) {
        // Returning the publish year of the publication
        return it->second.publishYear;
    }
    return NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end()) {
        // Returning a vector containing all of the publicationID's that reference this publication
        return it->second.affiliationsOfPub;
    }
    return {NO_AFFILIATION};
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it1 = publications.find(id);
    auto it2 = publications.find(parentid);

    // Checking that both the id's exist in the publications map
    if (it1 != publications.end() && it2!=publications.end()) {
        // Adding the parentid as the parent of the child id
        it1->second.parent.first = parentid;

        // Adding the parent publication as a pointer to the child id
        it1->second.parent.second = &publications.at(parentid);

        // Adding the child id and pointer to the child id
        it2->second.referencesOfPub.push_back(id);
        it2->second.children.push_back({id, &publications.at(id)});
        return true;
    }
    return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = publications.find(id);
    if (it != publications.end()) {
        // Returns a vector containing all of the publications that reference this publication
        return publications.at(id).referencesOfPub;
    }
    return {NO_PUBLICATION};
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto it1 = affiliations.find(affiliationid);
    auto it2 = publications.find(publicationid);
    if (it1 != affiliations.end() && it2!=publications.end()) {

        // Adding the affiliationID to the publication
        it2->second.affiliationsOfPub.push_back(affiliationid);

        // Adding the publicationID as well as the publish year to the affiliation
        it1->second.affiliatedPubs.push_back({publicationid, it2->second.publishYear});
        return true;
    }
    return false;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    // Find the affiliation in the affiliations map
    auto it = affiliations.find(id);

    // Vector to store the publicationIDs
    std::vector<PublicationID> affilPubIDs;

    // Check if the affiliation exists
    if (it != affiliations.end()) {
        for (const auto& part: it->second.affiliatedPubs) {
            affilPubIDs.push_back(part.first);
        }
        return affilPubIDs;
    }
    return {NO_PUBLICATION};
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto it = publications.find(id);
    // Checking that the publication has a parent and that it exists
    if (it != publications.end() && it->second.parent.second != nullptr) {
        return it->second.parent.first;
    }
    return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    // Find the affiliation in the affiliations map
    auto it = affiliations.find(affiliationid);

    // Check if the affiliation exists
    if (it != affiliations.end()) {
        // Vector to store publications after the given year
        std::vector<std::pair<Year, PublicationID>> publicationsAfter;

        // Iterate through affiliated publications of the affiliation
        for (const auto& part : it->second.affiliatedPubs) {
            // Check if the publication's year is greater than or equal to the given year
            if (part.second >= year) {
                // Add the publication to the vector with its year and ID
                publicationsAfter.push_back({part.second, part.first});
            }
        }
        // Sort the vector based on publication years and IDs
        std::sort(publicationsAfter.begin(), publicationsAfter.end(), [this] (const auto& ID1, const auto& ID2) {
            // Get publication years for comparison
            Year year1 = publications.at(ID1.second).publishYear;
            Year year2 = publications.at(ID2.second).publishYear;

            // If publication years are the same, compare publication IDs
            if (year1 == year2) {
                return ID1.second < ID2.second;
            }
            // Otherwise, compare publication years
            return year1 < year2;
        });
        // Return the sorted vector of publications after the given year
        return publicationsAfter;
    }
    // If affiliation not found, return a vector with these values
    return {{NO_YEAR ,NO_PUBLICATION}};
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    // Find the publication with the given ID in the publications map
    auto it = publications.find(id);

    // Check if the publication exists
    if (it != publications.end()) {
        // Vector to store the chain of publications referencing the given publication
        std::vector<PublicationID> referencedByChain = {};

        // Check if the publication has a parent (is referenced by another publication)
        if (it->second.parent.second == nullptr) {
            // If the publication has no parent, return an empty vector
            return referencedByChain;
        }
        // Starting with the parent of the given publication
        PublicationID nextID = it->second.parent.first;
        referencedByChain.push_back(nextID);

        // Get the next parent in the chain
        auto next = it->second.parent.second;

        // Iterating through the parent chain until there is no parent
        while (next != nullptr) {
            nextID = next->parent.first;

            // Checking if the next one is no publication
            if (nextID == NO_PUBLICATION) {
                // If no more parent, return vector
                return referencedByChain;
            }

            // Add the parent ID to the vector
            referencedByChain.push_back(nextID);

            // move to the next parent
            next = next->parent.second;
        }

        return referencedByChain;
    }

    return {NO_PUBLICATION};
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    // Find the publication with the given ID in the publications map
    auto it = publications.find(id);

    // Check if the publication exists
    if (it != publications.end()) {
        // Vector to store the IDs of all references
        std::vector<PublicationID> childrenVec = {};

        // Check if the publication has any children
        if (it->second.children.empty()) {
            // If no children, return an empty vector
            return childrenVec;
        }

        // Recursively retrieve all references starting from the given publication
        get_all_references_recursive(it->second, childrenVec);
        return childrenVec;
    }
    return {NO_PUBLICATION};
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    // Check if there are less than 2 affiliations
    if (affilIDVec.size() < 2) {
        // If so, return the existing affiliations
        return affilIDVec;
    }

    // Vector to store the closest affiliations
    std::vector<AffiliationID> closestAffiliations = {};

    // Sort affiliations based on the distance to the given coordinates so the closest ones are first
    std::sort(affilIDVec.begin(), affilIDVec.end(), [this, &xy] (const auto& ID1, const auto& ID2) {
        double distance1 = sqrt(pow((affiliations.at(ID1).coordinates.x-xy.x), 2) + pow((affiliations.at(ID1).coordinates.y-xy.y), 2));
        double distance2 = sqrt(pow((affiliations.at(ID2).coordinates.x-xy.x), 2) + pow((affiliations.at(ID2).coordinates.y-xy.y), 2));
        return distance1 < distance2;
    });

    // Check if there are less than 4 affiliations after sorting
    if (affilIDVec.size() < 4) {
        // If so, return the whole vector
        return affilIDVec;
    }

    // Retrieve the three closest affiliations
    for (int i = 0; i < 3; ++i) {
        closestAffiliations.push_back(affilIDVec.at(i));
    }

    return closestAffiliations;

}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    // Find the affiliation with the given ID
    auto it = affiliations.find(id);

    // Check if the affiliation exists
    if (it != affiliations.end()) {
        // Remove the affiliation ID from the vector
        affilIDVec.erase(std::remove(affilIDVec.begin(), affilIDVec.end(), id), affilIDVec.end());

        // Clear the sorted vectors and set bool values to false
        affilIDVecAlph.clear();
        affilIDVecDist.clear();
        alphabeticallySorted = false;
        distIncrSorted = false;

        // Iterate through affiliated publications and remove the affiliation ID
        int j = it->second.affiliatedPubs.size();
        for (int i = 0; i < j; ++i) {
            publications.at(it->second.affiliatedPubs.at(i).first).affiliationsOfPub.erase(
                        std::remove(publications.at(it->second.affiliatedPubs.at(i).first).affiliationsOfPub.begin(),
                                    publications.at(it->second.affiliatedPubs.at(i).first).affiliationsOfPub.end(), id),
                                    publications.at(it->second.affiliatedPubs.at(i).first).affiliationsOfPub.end());
        }

        // Find and erase the affiliation in alphabetic and distance maps
        auto it1 = affilAlphabetic.find(it->second.name);
        auto it2 = affilDistIncr.find(it->second.coordinates);
        affilAlphabetic.erase(it1);
        affilDistIncr.erase(it2);

        // Erase the affiliation from the main affiliations map
        affiliations.erase(id);

        // Return true meaning that the removal was succesful
        return true;
    }
    return false;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    // Find both of the publications
    auto it1 = publications.find(id1);
    auto it2 = publications.find(id2);

    // Check that both exist
    if (it1 != publications.end() && it2 != publications.end()) {
        // Get the chain of parent publications for both publications
        std::vector<PublicationID> parents1 = get_referenced_by_chain(id1);
        std::vector<PublicationID> parents2 = get_referenced_by_chain(id2);

        // Get the size of the parent chains
        int size1 = parents1.size();
        int size2 = parents2.size();

        // Iterate through the parent chains to find the closest common parent
        for (int i = 0; i < size1; ++i) {
            for (int j = 0; j < size2; ++j) {
                if (parents1.at(i) == parents2.at(j)) {
                    // Return the closest common parent
                    return parents1.at(i);
                }
            }
        }
    }

    // Return NO_PUBLICATION if no common parent found or publications not found
    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    // Find the publication and check it exists
    auto it = publications.find(publicationid);
    if (it != publications.end()) {
        // Create a pair for the publication ID and its publish year
        std::pair<PublicationID, Year> pairToRemove = {publicationid, it->second.publishYear};

        // Remove the publication from the affiliated publications of its affiliations
        int j = it->second.affiliationsOfPub.size();
        for (int i = 0; i < j; ++i) {
            affiliations.at(it->second.affiliationsOfPub.at(i)).affiliatedPubs.erase(
                        std::remove(affiliations.at(it->second.affiliationsOfPub.at(i)).affiliatedPubs.begin(),
                                    affiliations.at(it->second.affiliationsOfPub.at(i)).affiliatedPubs.end(), pairToRemove),
                        affiliations.at(it->second.affiliationsOfPub.at(i)).affiliatedPubs.end());
        }

        // Remove the publication ID from the vector of all publications
        publicationVec.erase(std::remove(publicationVec.begin(), publicationVec.end(), publicationid), publicationVec.end());

        // Update the parent information for the children of the publication
        int k = it->second.children.size();
        for (int i = 0; i < k; ++i) {
            it->second.children.at(i).second->parent = {NO_PUBLICATION,nullptr};
        }

        // Remove the publication from the publications map
        publications.erase(publicationid);

        return true;
    }
    return false;
}

void Datastructures::get_all_references_recursive(const Publication &publication, std::vector<PublicationID> &references)
{
    // Iterate over the children of the current publication
    for (const auto& child : publication.children) {
        // Add the child's ID to the references vector
        references.push_back(child.first);

        // Recursively call the function for the child's children
        get_all_references_recursive(*child.second, references);
    }
}


