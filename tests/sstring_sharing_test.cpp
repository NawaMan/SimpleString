#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <atomic>
#include "sstring.hpp"

// Task-003: String Sharing Optimization

namespace simple_string {

// Test fixture for accessing private members
class SStringSharing : public ::testing::Test {
protected:
    // Helper method to check if two strings share data
    bool sharingData(const SString& s1, const SString& s2) {
        return s1.sharesDataWith(s2);
    }
};

} // namespace simple_string

using namespace simple_string;

TEST_F(SStringSharing, CopyConstructorSharing) {
    // Create original string
    SString original("Hello");
    
    // Create copy through copy constructor
    SString copy(original);
    
    // They should share the same data
    EXPECT_TRUE(sharingData(original, copy));
}

TEST_F(SStringSharing, AssignmentSharing) {
    // Create two strings
    SString str1("Hello");
    SString str2("World");
    
    // Assignment should share data
    str2 = str1;
    EXPECT_TRUE(sharingData(str1, str2));
}

TEST_F(SStringSharing, IndependentStrings) {
    // Different content should not share data
    SString str1("Hello");
    SString str2("World");
    EXPECT_FALSE(sharingData(str1, str2));

    // Same content but independently constructed should not share
    SString str3("Hello");
    EXPECT_FALSE(sharingData(str1, str3));
}

TEST_F(SStringSharing, ThreadSafety) {
    const int NUM_THREADS = 10;
    const int ITERATIONS = 10000;  // Increased for better stress testing
    std::atomic<bool> failed{false};
    std::vector<std::thread> threads;

    // Create a shared string
    SString shared("Test String");
    
    // Function to test string operations
    auto test_func = [&]() {
        for (int i = 0; i < ITERATIONS && !failed; ++i) {
            // Create a copy (should be thread-safe)
            SString copy = shared;
            
            // Test sharing (should be thread-safe)
            if (!sharingData(copy, shared)) {
                failed = true;
                break;
            }

            // Test comparison (should be thread-safe)
            if (copy.compareTo(shared) != CompareResult::EQUAL) {
                failed = true;
                break;
            }

            // Add memory fence to ensure visibility across threads
            std::atomic_thread_fence(std::memory_order_seq_cst);
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

TEST_F(SStringSharing, ImmutabilityMaintained) {
    // Create a string
    SString str1("Hello");
    
    // Create a copy that shares the same data
    SString str2 = str1;

    // Verify they share the same data
    EXPECT_TRUE(sharingData(str1, str2));

    // Try to modify str1 through toString() (shouldn't be possible due to const)
    const std::string& str1_data = str1.toString();
    // The following line would not compile:
    // str1_data[0] = 'h';  // Error: str1_data is const

    // Data should still be shared and unchanged
    EXPECT_TRUE(sharingData(str1, str2));
    EXPECT_EQ(str1.toString(), "Hello");
    EXPECT_EQ(str2.toString(), "Hello");
}

TEST_F(SStringSharing, VectorCopies) {
    std::vector<SString> strings;
    
    // Add original string
    strings.push_back(SString("Hello"));
    
    // Add copies through push_back
    for (int i = 1; i < 5; ++i) {
        strings.push_back(strings[0]);
        EXPECT_TRUE(sharingData(strings[0], strings[i]))
            << "Copy " << i << " does not share data with original";
    }

    // Test that all copies share data with each other
    for (size_t i = 0; i < strings.size(); ++i) {
        for (size_t j = i + 1; j < strings.size(); ++j) {
            EXPECT_TRUE(sharingData(strings[i], strings[j]))
                << "String " << i << " does not share data with string " << j;
        }
    }
}

TEST_F(SStringSharing, EmptyStringSharing) {
    // Empty string constructed different ways
    SString empty1("");
    SString empty2("");
    
    // Create empty3 through conditional to test sharing behavior
    SString hello("hello");
    SString world("world");
    SString empty3 = hello.equals(world) ? empty1 : empty2; // Should use empty2
    
    SString empty4(empty1); // Copy constructor
    SString empty5 = empty2; // Copy assignment

    // All empty strings should be equal
    EXPECT_TRUE(empty1.equals(empty2));
    EXPECT_TRUE(empty2.equals(empty3));
    EXPECT_TRUE(empty3.equals(empty4));
    EXPECT_TRUE(empty4.equals(empty5));

    // But independently constructed empty strings might not share data
    // This is implementation-dependent, so we don't assert either way
    
    // Copied empty strings should share data
    EXPECT_TRUE(sharingData(empty1, empty4));
    EXPECT_TRUE(sharingData(empty2, empty5));
}

TEST_F(SStringSharing, NullCharacterSharing) {
    // Create strings with embedded null characters
    std::string str1("hello\0world", 11);
    std::string str2("hello\0world", 11);
    
    SString s1(str1);
    SString s2(str2);
    SString s3 = s1; // Copy of s1
    
    // Same content but independently constructed should not share
    EXPECT_FALSE(sharingData(s1, s2));
    
    // Copy should share
    EXPECT_TRUE(sharingData(s1, s3));
    
    // Verify the entire content is shared, including after null
    EXPECT_EQ(s1.toString().length(), 11);
    EXPECT_EQ(s3.toString().length(), 11);
    EXPECT_TRUE(std::equal(
        s1.toString().begin(), s1.toString().end(),
        s3.toString().begin()
    ));
}

TEST_F(SStringSharing, LongStringSharing) {
    // Create a very long string (1MB)
    const size_t size = 1024 * 1024;
    std::string longStr(size, 'x');
    
    // Create original and copy
    SString original(longStr);
    SString copy = original;
    
    // Verify sharing works for long strings
    EXPECT_TRUE(sharingData(original, copy));
    
    // Verify the entire content is shared
    EXPECT_EQ(original.toString().length(), size);
    EXPECT_EQ(copy.toString().length(), size);
    EXPECT_TRUE(std::equal(
        original.toString().begin(), original.toString().end(),
        copy.toString().begin()
    ));
}

TEST_F(SStringSharing, UnicodeSharing) {
    // Test with various Unicode sequences
    const char* utf8_str = "Hello 世界 🌍"; // Mix of ASCII, CJK, and emoji
    SString original(utf8_str);
    SString copy = original;
    
    // Verify sharing works with Unicode strings
    EXPECT_TRUE(sharingData(original, copy));
    
    // Verify the content is identical
    EXPECT_EQ(original.toString(), copy.toString());
    EXPECT_EQ(original.length(), copy.length());
    
    // Test with combining characters (e + combining acute)
    const char* combining = "e\xcc\x81"; // e + COMBINING ACUTE ACCENT (U+0301)
    SString combining1(combining);
    SString combining2 = combining1;
    
    EXPECT_TRUE(sharingData(combining1, combining2));
    EXPECT_EQ(combining1.length(), combining2.length());
}

TEST_F(SStringSharing, LengthCalculationBenchmark) {
    // Create strings with different characteristics
    std::vector<SString> testStrings = {
        SString(""),  // Empty
        SString("Hello, World!"),  // ASCII only
        SString("Hello, 世界!"),  // Mixed ASCII and CJK
        SString(std::string(1000, 'a')),  // Long ASCII
        SString("🌍🌎🌏"),  // All emoji (all surrogate pairs)
        SString("e\xcc\x81\xcc\x82\xcc\x83")  // Lots of combining chars
    };

    const int ITERATIONS = 100000;
    
    // Time length() calls
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile size_t total = 0;  // Prevent optimization
    for (int i = 0; i < ITERATIONS; ++i) {
        for (const auto& str : testStrings) {
            total += str.length();  // Force length calculation
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double avgMicrosPerCall = static_cast<double>(duration.count()) / (ITERATIONS * testStrings.size());
    
    std::cout << "\nLength Calculation Benchmark:\n"
              << "  Total calls: " << (ITERATIONS * testStrings.size()) << "\n"
              << "  Total time: " << duration.count() << " microseconds\n"
              << "  Average time per call: " << avgMicrosPerCall << " microseconds\n";

    // Verify results are consistent
    std::vector<size_t> lengths;
    for (const auto& str : testStrings) {
        size_t len = str.length();
        for (int i = 0; i < 10; ++i) {
            EXPECT_EQ(str.length(), len) << "Length calculation inconsistent";
        }
    }
}
