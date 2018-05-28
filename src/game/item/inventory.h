#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "item.h"

/**
INventory is not used in demo version for lack of UI
**/

class Inventory {

public:

	class Entry {

		friend class Inventory;

		public: 

			long getId() const { return id_; };

			int getCount() const { return count_; };

			const Item* getItem() const { return item_; }

		private: 

			static long next_id;

			static long getNextId();

			Entry(const Item* item) : count_(1), item_(item), id_(getNextId()) {};
			
			long id_;

			int count_;

			const Item* item_;

	};

	Inventory();

	bool canAddItem(const Item* item) const;

	void addItem(const Item* item);

	void removeItem(const Item* item);

	std::vector<Entry> getEntries(int item_class) const;

	void debug_print() const;

private:

	void sort();

	void sort(int group);

	const Entry* getAvailableEntry(const Item* item) const; 

	Entry* getAvailableEntry(const Item* item);

	int stack_limit_;

	std::vector<int> slot_limits_;

	std::vector<std::vector<Entry>> items_;

};

#endif