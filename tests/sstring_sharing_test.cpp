#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <atomic>
#include "sstring.hpp"

// Task-003: Test shared_ptr-based string sharing through copy and assignment

using namespace simple_string;

TEST(SStringSharing, CopyConstructorSharing) {
    // Create original string
    SString original("Hello");
    
    // Create copy through copy constructor
    SString copy(original);
    
    // They should share the same data
    EXPECT_TRUE(original.sharesDataWith(copy));
}

TEST(SStringSharing, AssignmentSharing) {
    // Create two strings
    SString str1("Hello");
    SString str2("World");
    
    // Assignment should share data
    str2 = str1;
    EXPECT_TRUE(str1.sharesDataWith(str2));
}

TEST(SStringSharing, IndependentStrings) {
    // Different content should not share data
    SString str1("Hello");
    SString str2("World");
    EXPECT_FALSE(str1.sharesDataWith(str2));

    // Same content but independently constructed should not share
    SString str3("Hello");
    EXPECT_FALSE(str1.sharesDataWith(str3));
}

TEST(SStringSharing, ThreadSafety) {
    const int NUM_THREADS = 10;
    const int ITERATIONS = 1000;
    std::atomic<bool> failed{false};
    std::vector<std::thread> threads;

    // Create a shared string
    SString shared("Test String");
    
    // Function to test string operations
    auto test_func = [&]() {
        for (int i = 0; i < ITERATIONS && !failed; ++i) {
            // Create a copy (should be thread-safe)
            SString copy = shared;
            
            // Test equality (should be thread-safe)
            if (!copy.equals(shared)) {
                failed = true;
                break;
            }

            // Test comparison (should be thread-safe)
            if (copy.compareTo(shared) != CompareResult::EQUAL) {
                failed = true;
                break;
            }
        }
    };

    // Start threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(test_func);
    }

    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_FALSE(failed) << "Thread safety test failed";
}

TEST(SStringSharing, ImmutabilityMaintained) {
    // Create a string
    SString str1("Hello");
    
    // Create a copy that shares the same data
    SString str2 = str1;

    // Verify they share the same data
    EXPECT_TRUE(str1.sharesDataWith(str2));

    // Try to modify str1 through toString() (shouldn't be possible due to const)
    const std::string& str1_data = str1.toString();
    // The following line would not compile:
    // str1_data[0] = 'h';  // Error: str1_data is const

    // Data should still be shared and unchanged
    EXPECT_TRUE(str1.sharesDataWith(str2));
    EXPECT_EQ(str1.toString(), "Hello");
    EXPECT_EQ(str2.toString(), "Hello");
}

TEST(SStringSharing, VectorCopies) {
    std::vector<SString> strings;
    
    // Add original string
    strings.push_back(SString("Hello"));
    
    // Add copies through push_back
    for (int i = 1; i < 5; ++i) {
        strings.push_back(strings[0]);
        EXPECT_TRUE(strings[0].sharesDataWith(strings[i]))
            << "Copy " << i << " does not share data with original";
    }

    // Test that all copies share data with each other
    for (size_t i = 0; i < strings.size(); ++i) {
        for (size_t j = i + 1; j < strings.size(); ++j) {
            EXPECT_TRUE(strings[i].sharesDataWith(strings[j]))
                << "String " << i << " does not share data with string " << j;
        }
    }
}
