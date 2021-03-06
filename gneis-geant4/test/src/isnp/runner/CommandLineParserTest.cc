#include <string.h>
#include <memory.h>
#include <vector>

#include <gtest/gtest.h>
#include "isnp/runner/CommandLineParser.hh"

namespace isnp {

namespace runner {

class Helper {
public:

	Helper() = delete;

	static CommandLineParser Instance(const char* const commandLine) {
		// container for all arguments
		std::vector<char*> arguments;
		// first argument is a path to executable
		arguments.push_back(::strdup("/path/to/executable")); //	don't care about memory freeing

		// split command line by spaces
		char* const buf = ::strdup(commandLine); //	don't care about memory freeing
		char* p = ::strtok(buf, " ");
		while (p != nullptr) {
			arguments.push_back(p);
			p = strtok(nullptr, " ");
		}

		// number of arguments
		int const argc = arguments.size();

		// argument values
		char** const argv = (char**) ::calloc(argc, sizeof(char*)); //	don't care about memory freeing
		memcpy(argv, &arguments[0], argc * sizeof(char*));

		return CommandLineParser(arguments.size(), argv, true);
	}
};

TEST(CommandLineParser, Nothing) {
	CommandLineParser const parser = Helper::Instance("");
	EXPECT_EQ(0, parser.GetReturnCode());
	EXPECT_EQ(1, parser.GetArgc());
	EXPECT_FALSE(parser.GetVisualMode());
	EXPECT_STREQ("/path/to/executable", parser.GetArgv()[0]);
}

TEST(CommandLineParser, NoOptions) {
	CommandLineParser const parser = Helper::Instance("filename1 filename2");
	EXPECT_EQ(0, parser.GetReturnCode());
	EXPECT_EQ(3, parser.GetArgc());
	EXPECT_FALSE(parser.GetVisualMode());
	EXPECT_STREQ("/path/to/executable", parser.GetArgv()[0]);
	EXPECT_STREQ("filename1", parser.GetArgv()[1]);
	EXPECT_STREQ("filename2", parser.GetArgv()[2]);
}

TEST(CommandLineParser, Help) {
	{
		CommandLineParser const parser = Helper::Instance("-h");
		EXPECT_EQ(1, parser.GetReturnCode());
		EXPECT_EQ(1, parser.GetArgc());
	}
	{
		CommandLineParser const parser = Helper::Instance("-?");
		EXPECT_EQ(1, parser.GetReturnCode());
		EXPECT_EQ(1, parser.GetArgc());
	}
}

#ifdef G4MULTITHREADED

TEST(CommandLineParser, NumOfThreads) {
	{
		CommandLineParser const parser = Helper::Instance("filename1 filename2");
		EXPECT_EQ(0, parser.GetReturnCode());
		EXPECT_EQ(3, parser.GetArgc());
		EXPECT_EQ(0, parser.GetNumOfThreads());
		EXPECT_STREQ("filename1", parser.GetArgv()[1]);
		EXPECT_STREQ("filename2", parser.GetArgv()[2]);
	}

	{
		CommandLineParser const parser = Helper::Instance("-t 4 filename1 filename2");
		EXPECT_EQ(0, parser.GetReturnCode());
		EXPECT_EQ(3, parser.GetArgc());
		EXPECT_EQ(4, parser.GetNumOfThreads());
		EXPECT_STREQ("filename1", parser.GetArgv()[1]);
		EXPECT_STREQ("filename2", parser.GetArgv()[2]);
	}
}

#endif

TEST(CommandLineParser, AllOptions) {
	CommandLineParser const parser = Helper::Instance("-v filename1 filename2");
	EXPECT_EQ(0, parser.GetReturnCode());
	EXPECT_EQ(3, parser.GetArgc());
	EXPECT_TRUE(parser.GetVisualMode());
	EXPECT_STREQ("filename1", parser.GetArgv()[1]);
	EXPECT_STREQ("filename2", parser.GetArgv()[2]);
}

TEST(CommandLineParser, BadOption) {
	CommandLineParser const parser = Helper::Instance("-Z");
	EXPECT_EQ(1, parser.GetReturnCode());
	EXPECT_EQ(1, parser.GetArgc());
}

}

}
