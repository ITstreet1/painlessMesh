#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#define ARDUINOJSON_USE_LONG_LONG 1
#include "ArduinoJson.h"
#undef ARDUINOJSON_ENABLE_ARDUINO_STRING
#undef PAINLESSMESH_ENABLE_ARDUINO_STRING
#define PAINLESSMESH_ENABLE_STD_STRING
typedef std::string TSTRING;

#include "catch_utils.hpp"
#include "fake_serial.h"

#define TASK_FOREVER 0
#define TASK_ONCE 0
#define TASK_MILLISECOND 1
#define TASK_SECOND 1000 * TASK_MILLISECOND
#define TASK_MINUTE 60 * TASK_SECOND

#include "painlessmesh/router.hpp"

using namespace painlessmesh;

logger::LogClass Log;

SCENARIO("CallbackMap should hold multiple callbacks by ID") {
  GIVEN("A callback map with added callbacks") {
    auto cbl = router::CallbackList<int>();

    auto i = 0;
    auto j = 0;

    cbl.onPackage(1, [&i](int z) {
      ++i;
      return false;
    });
    cbl.onPackage(1, [&j](int z) {
      ++j;
      return false;
    });

    WHEN("We call execute") {
      auto cnt = cbl.execute(1, 0);
      REQUIRE(cnt == 2);
      THEN("The callbacks are called") {
        REQUIRE(i == 1);
        REQUIRE(j == 1);
      }
    }

    WHEN("We call execute on another event") {
      auto cnt = cbl.execute(2, 0);
      REQUIRE(cnt == 0);
      THEN("The callbacks are not called") {
        REQUIRE(i == 0);
        REQUIRE(j == 0);
      }
    }

    cbl.onPackage(1, [&i](int z) {
      ++i;
      return true;
    });
    cbl.onPackage(1, [&j](int z) {
      ++j;
      return false;
    });
    WHEN("We call execute it will stop on a return of true") {
      auto cnt = cbl.execute(1, 3);
      REQUIRE(cnt == 3);
      THEN("The callbacks are called") {
        REQUIRE(i == 2);
        REQUIRE(j == 1);
      }
    }
  }
}
/*
class MockConnection : public protocol::NodeTree {
 public:
  void addMessage(TSTRING msg, bool priority = false) { ++cnt; }

  int cnt = 0;
};

SCENARIO("findRoute works as expected with different types of connections") {
  GIVEN("A layout with Neighbour shared ptrs") {
    auto layout = layout::Layout<layout::Neighbour>();
    std::string jsonTree1 =
        "{\"type\":6,\"dest\":2428398258,\"from\":3107768579,\"nodeId\":"
        "3907768579,\"subs\":[{\"nodeId\":3959373838,\"subs\":[{\"nodeId\":"
        "416992913},{\"nodeId\":1895675348}]}]}";
    auto variant1 = protocol::Variant(jsonTree1);
    auto tree1 =
        std::make_shared<layout::Neighbour>(variant1.to<layout::Neighbour>());

    std::string jsonTree2 =
        "{\"type\":6,\"dest\":2428398258,\"from\":3107768579,\"nodeId\":"
        "3907768580,\"subs\":[{\"nodeId\":3959373839,\"subs\":[{\"nodeId\":"
        "416992914},{\"nodeId\":1895675349}]}]}";
    auto variant2 = protocol::Variant(jsonTree2);
    auto tree2 =
        std::make_shared<layout::Neighbour>(variant2.to<layout::Neighbour>());

    std::string jsonTree3 =
        "{\"type\":6,\"dest\":2428398258,\"from\":3107768579,\"nodeId\":"
        "3907768581,\"subs\":[{\"nodeId\":3959373840,\"subs\":[{\"nodeId\":"
        "416992915},{\"nodeId\":1895675350}]}]}";
    auto variant3 = protocol::Variant(jsonTree3);
    auto tree3 =
        std::make_shared<layout::Neighbour>(variant3.to<layout::Neighbour>());

    layout.subs.push_back(tree1);
    layout.subs.push_back(tree2);
    layout.subs.push_back(tree3);

    layout.nodeId = runif(1, 1000);

    THEN("findRoute works") {
      auto rt = router::findRoute<layout::Neighbour>(layout, 1895675350);
      REQUIRE(rt->nodeId == 3907768581);
      rt = router::findRoute<layout::Neighbour>(layout, 1895675351);
      REQUIRE(!rt);
    }

    THEN("It can be converted to a NodeTree") {
      auto lay = layout.asNodeTree();
      auto nt = protocol::NodeTree();
      nt.nodeId = lay.nodeId;
      for (auto &&s : lay.subs) {
        nt.subs.push_back(s);
      }
      REQUIRE(nt == lay);
    }
  }
}

SCENARIO("routePackage should route the package correctly") {
  GIVEN("A CallbackList and layout") {
    auto cbl = router::CallbackList();
    auto lay = layout::Layout<MockConnection>();
    lay.nodeId = 1;
    lay.subs.push_back(std::make_shared<MockConnection>());
    lay.subs.back()->nodeId = 2;
    lay.subs.push_back(std::make_shared<MockConnection>());
    lay.subs.back()->nodeId = 3;
    lay.subs.push_back(std::make_shared<MockConnection>());
    lay.subs.back()->nodeId = 4;

    // void routePackage(Layout<T>, T conn, TSTRING pkg, CallbackMap)
    WHEN("Passed a package with routing NEIGHBOUR") {
      auto pkg = createTimeSync();
      TSTRING str;
      auto var = protocol::Variant(pkg);
      REQUIRE(var.routing() == router::NEIGHBOUR);
      var.printTo(str);
      // message type NEIGHBOUR should result in a callback
    }

    // message type BROADCAST should result in a callback and being send on

    // message type SINGLE if destination is other then send otherwise callback
  }
}
*/
