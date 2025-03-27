#include "../include/string.hpp"
#include <atomic>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

// Task-003: String Sharing Optimization

namespace simple {

// Test fixture for accessing private members
class StringSharing : public ::testing::Test {
protected:
	// Helper method to check if two strings share data
	bool sharingData(const String &s1, const String &s2) {
		return s1.shares_data_with(s2);
	}
};

TEST_F(StringSharing, CopyConstructorSharing) {
	// Create original string
	String original("Hello");

	// Create copy through copy constructor
	String copy(original);

	// They should share the same data
	EXPECT_TRUE(sharingData(original, copy));
}

TEST_F(StringSharing, AssignmentSharing) {
	// Create two strings
	String str1("Hello");
	String str2("World");

	// Assignment should share data
	str2 = str1;
	EXPECT_TRUE(sharingData(str1, str2));
}

TEST_F(StringSharing, IndependentStrings) {
	// Different content should not share data
	String str1("Hello");
	String str2("World");
	EXPECT_FALSE(sharingData(str1, str2));

	// Same content but independently constructed should not share
	String str3("Hello");
	EXPECT_FALSE(sharingData(str1, str3));
}

TEST_F(StringSharing, ThreadSafety) {
	const int NUM_THREADS = 10;
	const int ITERATIONS = 10000; // Increased for better stress testing
	std::atomic<bool> failed{false};
	std::vector<std::thread> threads;

	// Create a shared string
	String shared("Test String");

	// Function to test string operations
	auto test_func = [&]() {
		for (int i = 0; i < ITERATIONS && !failed; ++i) {
			// Create a copy (should be thread-safe)
			String copy = shared;

			// Test sharing (should be thread-safe)
			if (!sharingData(copy, shared)) {
				failed = true;
				break;
			}

			// Test comparison (should be thread-safe)
			if (copy.compare_to(shared) != CompareResult::EQUAL) {
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
	for (auto &thread : threads) {
		thread.join();
	}

	EXPECT_FALSE(failed) << "Thread safety test failed";
}

TEST_F(StringSharing, ImmutabilityMaintained) {
	// Create a string
	String str1("Hello");

	// Create a copy that shares the same data
	String str2 = str1;

	// Verify they share the same data
	EXPECT_TRUE(sharingData(str1, str2));

	// Try to modify str1 through toString() (shouldn't be possible due to
	// const)
	const std::string &str1_data = str1.to_string();
	// The following line would not compile:
	// str1_data[0] = 'h';  // Error: str1_data is const

	// Data should still be shared and unchanged
	EXPECT_TRUE(sharingData(str1, str2));
	EXPECT_EQ(str1.to_string(), "Hello");
	EXPECT_EQ(str2.to_string(), "Hello");
}

TEST_F(StringSharing, VectorCopies) {
	std::vector<String> strings;

	// Add original string
	strings.push_back(String("Hello"));

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

TEST_F(StringSharing, EmptyStringSharing) {
	// Empty string constructed different ways
	String empty1("");
	String empty2("");

	// Create empty3 through conditional to test sharing behavior
	String hello("hello");
	String world("world");
	String empty3 = hello.equals(world) ? empty1 : empty2; // Should use empty2

	String empty4(empty1);	// Copy constructor
	String empty5 = empty2; // Copy assignment

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

TEST_F(StringSharing, NullCharacterSharing) {
	// Create strings with embedded null characters
	std::string str1("hello\0world", 11);
	std::string str2("hello\0world", 11);

	String s1(str1);
	String s2(str2);
	String s3 = s1; // Copy of s1

	// Same content but independently constructed should not share
	EXPECT_FALSE(sharingData(s1, s2));

	// Copy should share
	EXPECT_TRUE(sharingData(s1, s3));

	// Verify the entire content is shared, including after null
	EXPECT_EQ(s1.to_string().length(), 11);
	EXPECT_EQ(s3.to_string().length(), 11);
	EXPECT_TRUE(std::equal(s1.to_string().begin(), s1.to_string().end(),
						   s3.to_string().begin()));
}

TEST_F(StringSharing, LongStringSharing) {
	// Create a very long string (1MB)
	const size_t size = 1024 * 1024;
	std::string longStr(size, 'x');

	// Create original and copy
	String original(longStr);
	String copy = original;

	// Verify sharing works for long strings
	EXPECT_TRUE(sharingData(original, copy));

	// Verify the entire content is shared
	EXPECT_EQ(original.to_string().length(), size);
	EXPECT_EQ(copy.to_string().length(), size);
	EXPECT_TRUE(std::equal(original.to_string().begin(),
						   original.to_string().end(),
						   copy.to_string().begin()));
}

TEST_F(StringSharing, UnicodeSharing) {
	// Test with various Unicode sequences
	const char *utf8_str = "Hello 世界 🌍"; // Mix of ASCII, CJK, and emoji
	String original(utf8_str);
	String copy = original;

	// Verify sharing works with Unicode strings
	EXPECT_TRUE(sharingData(original, copy));

	// Verify the content is identical
	EXPECT_EQ(original.to_string(), copy.to_string());
	EXPECT_EQ(original.length(), copy.length());

	// Test with combining characters (e + combining acute)
	const char *combining = "e\xcc\x81"; // e + COMBINING ACUTE ACCENT (U+0301)
	String combining1(combining);
	String combining2 = combining1;

	EXPECT_TRUE(sharingData(combining1, combining2));
	EXPECT_EQ(combining1.length(), combining2.length());
}

TEST_F(StringSharing, LengthCalculationBenchmark) {
	// Create strings with different characteristics
	std::vector<String> testStrings = {
		String(""),							// Empty
		String("Hello, World!"),			// ASCII only
		String("Hello, 世界!"),				// Mixed ASCII and CJK
		String(std::string(1000, 'a')),		// Long ASCII
		String("🌍🌎🌏"),					// All emoji (all surrogate pairs)
		String("e\xcc\x81\xcc\x82\xcc\x83") // Lots of combining chars
	};

	const int ITERATIONS = 100000;

	// Time length() calls
	auto start = std::chrono::high_resolution_clock::now();

	volatile size_t total = 0; // Prevent optimization
	for (int i = 0; i < ITERATIONS; ++i) {
		for (const auto &str : testStrings) {
			total += str.length(); // Force length calculation
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration =
		std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	double avgMicrosPerCall = static_cast<double>(duration.count()) /
							  (ITERATIONS * testStrings.size());

	std::cout << "\nLength Calculation Benchmark:\n"
			  << "  Total calls: " << (ITERATIONS * testStrings.size()) << "\n"
			  << "  Total time: " << duration.count() << " microseconds\n"
			  << "  Average time per call: " << avgMicrosPerCall
			  << " microseconds\n";

	// Verify results are consistent
	std::vector<size_t> lengths;
	for (const auto &str : testStrings) {
		size_t len = str.length();
		for (int i = 0; i < 10; ++i) {
			EXPECT_EQ(str.length(), len) << "Length calculation inconsistent";
		}
	}
}

} // namespace simple