// Modified by:   ____________Branden Burke__________
// Lab:           ____________2________ Due: Oct. 11th @ 11:59 pm
// Course number: ____________CIS-221________
/* !!!!! INTEGRITY STATEMENT !!!!!
  “I either typed in all the following code myself or copied from my own 
  earlier CIS-221 assignments or from files on our textbook website.
  I did not copy code from other sources or people, and 
  I did not share my code with anyone else.”  
*/

// FILE: sequence.cpp (implementation file)
// CLASS PROVIDED: sequence (part of the namespace main_savitch_4)

// INVARIANT for the sequence:
//
//   1. The number of value_types in the sequence is in the member variable used
//   2. For a non-empty sequence, the value_types in the sequence are
//      stored in order in data[0] through data[used-1].
//   3. current_index keeps track of the current location in data; whenever current_index
//      is greater than or equal to used, there is no current value_type.
//   4. capacity keeps track of the current capacity of the array pointed
//      to by data.  Capacity starts out with the value initial_capacity
//      and is changed by resize as needed (whenever used is greater than
//      or equal to capacity).

// POINTERS are used in this program to implement a sequence. Therefore,
// things will be dynamically allocated. As a result, there are three
// things that will be required: destructor, copy constructor, and 
// assignment operator (overloaded). Additionally, the Linux tool
// valgrind will be used to check for memory leaks. Thus, I will have
// to test this program on my VM through VirtualBox after or during the 
// programming process. Preferably during since it will be harder to debug
// later on in the coding process.

#include <cassert>   // Provides assert
#include "sequence.h" // provides the header file
#include <algorithm> // provides the copy function
using namespace std;

namespace main_savitch_4
{
  // I am not entirely sure why this is included, but I 
  // chose to include it because this was used in the
  // dynamic bag implementation.
  const sequence::size_type sequence::DEFAULT_CAPACITY;

  sequence::sequence(size_type initial_capacity)      // the main constructor
  {
    // Library facilities used: cstdlib (included in the file sequence.h)
    // not a default constructor, no need for one since it is already
    // initialized if no argument is passed (e.g. DEFAULT_CAPACITY = 30)
    // In other words, the regular constructor also does the job of the 
    // default constructor since it's parameter has a default argument. 

    // no need to update index at this point b/c that is the start()
    // functions job
    data = new value_type[initial_capacity];  // dynamic array & ptr is set
    capacity = initial_capacity;
    used = 0;

    // DONE? Yes!
  }

  sequence::sequence(const sequence& source)          //  the copy constructor
  {
    // Library facilities used: cstdlib
    // deep copy, not a sallow copy
    data = new value_type[source.capacity]; // another dynamic array
    capacity = source.capacity;
    used = source.used;
    copy(source.data, source.data + used, data); // begin, end, where
    // does order matter here? maybe

    // DONE? Yes!
  }

  sequence::~sequence()                           // the destructor
  {
    // doesn't the destructor take care of itself?
    // In other words, the programmer never has to 
    // explicitly call it? NO, return to the heap
    delete [] data;

    // DONE? Yes!
  }

  void sequence::start()
  {
    // goal: 1st item becomes the index (start at begining, 0)

    // if there is no item there is no index?
    current_index = 0;

    // DONE? Yes!
  }

  void sequence::advance()
  {
    // goal: move index forward one
    assert(is_item());
    ++current_index;

    // DONE? Yes!
  }

  void sequence::insert(const value_type& entry)
  {
    // goal: place element to the left

    // if full add another spot
    if(used == capacity)
    {
      // Used should not be incremented before because it is not only 
      // being passed through resize() as an argument, but it also
      // would mess up with the used value after the resize() has been called.
      // e.g. ++used should not be used before then passed
      resize(used + 1);
    }

    // start at end and shift everything to the right
    if (used > 0)
    {
      // Similar to what was on our 1st exam, order matters here and an INVALID INDEX
      // should never be used. Therefore, the second condition is added in order to
      // prevent the use of an invalid index (e.g. when current_index is zero and
      // the loop reaches the final spot which is beyond/before the sequence 
      // such as - 1 index should not be used). Both conditions have to be true 
      // for the loop to iterate.

      // equal to 
      // because we also need to move the value at current_index
      for(size_type i = used - 1; (i >= current_index) && (i != current_index - 1); --i)
      {
        data[i + 1] = data[i];
      }

      // now availabe to insert 
      data[current_index] = entry;
      ++used;
      // incrementing used serves two purposes:
      // one - it increases the size
      // two - it allows for the assignment of no item later on
    }
    else
    {
      // all other sinerios (such as used being equal to zero)
      data[current_index] = entry;
      ++used;
    }
  
    // DONE? Yes! 
  }

  void sequence::attach(const value_type& entry)
  {
    // goal: place element to the right

    // if full add another spot
    if (used == capacity)
    {
      // Again, should not increment used before because it will 
      // mess with the value of used, which is not what we want.
      // Remember, the goal is to increase the cpacity and not used.
      resize(used + 1);
    }

    if (used > 0)
    {
      // start at end and shift everything to the right
      for (size_type i = used - 1; i > current_index; --i)  // not equal too
      {
        data[i + 1] = data[i];
      }
      
      // now availabe to attach
      // increment current index before because we want the new element that
      // was added to be the current_index afterwards
      ++current_index;
      data[current_index] = entry;
      ++used; 
    } 
    else
    {
      // all other sinerios (such as used being equal to zero)
      data[current_index] = entry;
      ++used;
    }

    // DONE? Yes!
  }

  void sequence::remove_current()
  {
    // goal: remove the element at the current index

    assert(is_item());

    // start at the next slot and shift everything to the left
    // thus erasing by overriding the element at the current index
    for (size_type i = current_index + 1; i < used; ++i)
    {
      data[i - 1] = data[i];
    }
    --used;

    // DONE? Yes!
  }

  // imported from dynamic bag2
  void sequence::resize(size_type new_capacity)
  {
    // goal: increase the size of the given array (resize == reserve)

    value_type *larger_array;

    if (new_capacity == capacity)
    {
      return; 
    }
    // The allocated memory is already the right size.
    // prevents unnecessary code from running; thus, more efficient

    if (new_capacity < used)
    {
      new_capacity = used; // Cannot allocate less than we are using.
    }

    // the five steps discussed in class
    larger_array = new value_type[new_capacity];
    copy(data, data + used, larger_array);
    delete [ ] data;
    data = larger_array;
    capacity = new_capacity;

    // DONE? Yes!
  }

  // imported from dynamic bag2
  void sequence::operator =(const sequence& source)
  {
    // goal: overload the assignment operator to work with seq(s)

    // temp array to avoid catastrophy
    value_type *new_data;

    // Check for possible self-assignment:
    if (this == &source)
    {
      return;
    }

    // If needed, allocate an array with a different size:
    if (capacity != source.capacity)
    { 
      new_data = new value_type[source.capacity];
      delete [ ] data;  // deletion of the old array
      data = new_data;  // redirection of ptr
      capacity = source.capacity;
    }

    // Copy the data from the source array:
    used = source.used;
    current_index = source.current_index;
    copy(source.data, source.data + used, data);

    // DONE? Yes!
  }

  sequence::size_type sequence::size() const
  {
    // goal: return the size of the array
    return used;

    // DONE? Yes!
  }

  bool sequence::is_item() const
  {
    // goal: return if the array contains an element at the index
    // true means there are elemnets
    // false means there are NOT elements
    return (current_index < used);

    // DONE? Yes!
  }

  sequence::value_type sequence::current() const
  {
    // goal: return the element at the current index
    assert(is_item());
    return data[current_index];

    // DONE? Yes, if element is what the postcondition means by current item
  }

  // added these for myself (testing purposes)
  sequence::value_type sequence::current_i() const
  {
    return current_index;
    // works
  }

  sequence::value_type sequence::current_cap() const
  {
    return capacity;
    // works
  }

}