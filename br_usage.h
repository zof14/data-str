#ifndef BR_USAGE_H
#define BR_USAGE_H
#include "bi_ring.h"
#include <unordered_map>

// Filtering
// Resulting collection contains only the Key – Info pairs for which given predicate is true.
template<typename Key, typename Info>
bi_ring<Key, Info> filter(const bi_ring<Key, Info>& source, bool (pred)(const Key&)){
	bi_ring<Key, Info> res;
    if (source.empty()) {
        return res; 
    }
    typename bi_ring<Key, Info>::const_iterator iter = source.cbegin();
    typename bi_ring<Key, Info>::const_iterator start = iter; 

    do {
        if (pred(iter.key())) {
            res.push_back(iter.key(), iter.info());  
        }
        ++iter;
    } while (iter != start);

    return res;
}


// Joining
// Joins two rings summing Info if the same key is present in both rings
// (Info must have operator + defined)
template <typename Key, typename Info>
bi_ring<Key, Info> join(const bi_ring<Key, Info> &first,
                        const bi_ring<Key, Info> &second)
{
 	bi_ring<Key, Info> result;

    std::unordered_map<Key, Info> joined_values;
    if (!first.empty()) {
        typename bi_ring<Key, Info>::const_iterator it1 = first.cbegin();
        do {
            Key key = it1.key();
            Info info = it1.info();

            if (joined_values.find(key) != nullptr) {
                joined_values[key]= joined_values[key]+info; 
            } else {
                joined_values[key] = info;
            }

            ++it1;
        } while (it1 != first.cbegin());
    }
    if (!second.empty()) {
        typename bi_ring<Key, Info>::const_iterator it2 = second.cbegin();
        do {
            Key key = it2.key();
            Info info = it2.info();

            if (joined_values.find(key) != nullptr) {
                joined_values[key]= joined_values[key]+info;
            } else {
                joined_values[key] = info;
            }

            ++it2;
        } while (it2 != second.cbegin());
    }
    for (const auto &pair : joined_values) {
        result.push_back(pair.first, pair.second);
    }

    return result;
}
// Unique
// Eliminates repetitions of the key value
// Resulting collection contains elemenst with unique keys.
// aggregate function is used to compute new info value for two elements with the same key.
template<typename Key, typename Info>
bi_ring<Key, Info> unique(const bi_ring<Key, Info>& source,Info(aggregate)(const Key&, const Info&, const Info&)){
		bi_ring<Key, Info> res;
		if(source.empty()){
			return res;
		}
		typename bi_ring<Key, Info>::const_iterator  iter = source.cbegin();
    	typename bi_ring<Key, Info>::const_iterator  start = iter; 
		do{
			const Key& k= iter.key();
        	const Info& inf = iter.info();
			int is_present=0;
			typename bi_ring<Key, Info>::iterator  start2=res.begin();
			typename bi_ring<Key, Info>::iterator result_iter=res.begin();
			do{
				if(res.empty()){
					break;}
				if(result_iter.key()==k){
					is_present=1;
					result_iter.set_info(aggregate(k, result_iter.info(), inf));
					break;
				}
				++result_iter;


			}while(result_iter!=start2);
			if (is_present==0){
				res.push_back(k, inf);
			}
			++iter;

		}while(iter!=start);
		return res;

};



// Shuffle
// Very strange join (shuffle) operation taking fcnt elements from the first bi_ring
// and scnt elements from the second bi_ring (repeated reps times)
// If we reach the end of the collection and need more elements,
// we start again from the beginning of the collection - cyclically
template<typename Key, typename Info>
bi_ring<Key, Info> shuffle(
	const bi_ring<Key, Info>& first, unsigned int fcnt,
	const bi_ring<Key, Info>& second, unsigned int scnt,unsigned int reps){

		bi_ring<Key, Info> res;
		typename bi_ring<Key, Info>::const_iterator it=first.cbegin();
		typename bi_ring<Key, Info>::const_iterator it2=second.cbegin();
		while(reps>0){
			 std::cout << "Reps remaining: " << reps << std::endl;

			
			if(first.empty() && second.empty()){
				return res;
			}
			
			for(unsigned int count=0;count<fcnt;count++){
				if(first.empty()){
					break;}
				std::cout << "Adding from first: Key = " << it.key() << ", Info = " << it.info() << std::endl;
				res.push_back(it.key(),it.info());
				++it;
			}
			for(unsigned int count=0;count<scnt;count++){
				if(second.empty()){
					break;}
				std::cout << "Adding from second: Key = " << it2.key() << ", Info = " << it2.info() << std::endl;
				res.push_back(it2.key(), it2.info());
				++it2;
			}
			--reps;}
		return res;

	};
#endif