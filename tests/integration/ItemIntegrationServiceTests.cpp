//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../../src/Service/ItemService.h"

class ItemIntegrationServiceTests : public testing::Test {

protected:
    ItemService itemService;

    void addItems(){
        itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "b", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "c", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "d", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    }

public:
    virtual ~ItemIntegrationServiceTests() override {
        itemService.process(ItemDeleteAllReq{});
    }
};

class ServiceIntegrationTestsItemServiceRequestId
        : public ItemIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationRegistrationReqestId, ServiceIntegrationTestsItemServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceIntegrationTestsItemServiceRequestId, ItemServiceIntegrationRegistrationReqestId) {
    auto const& id = GetParam();
    auto request = AddItemReq{.requestId = id, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty};
    auto responce = itemService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceIntegrationTestsItemServiceRegistrationValid
        : public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, uint64_t, Category>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationRegistrationValid, ServiceIntegrationTestsItemServiceRegistrationValid,
        ::testing::Values(
                std::make_tuple("My name is Liza", "many words", 1, Category::beauty),
                std::make_tuple("Liza", "desc", 10, Category::technic),
                std::make_tuple("Л", "07-05-2004", 10, Category::clothes),
                std::make_tuple("Liza", "", 10, Category::technic),
                std::make_tuple("Makar", "   ", 10, Category::unknown),
                std::make_tuple("Makar", "04-05-1070", 10, Category::unknown))
                );

TEST_P(ServiceIntegrationTestsItemServiceRegistrationValid, ItemServiceIntegrationRegistrationValid) {
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceIntegrationTestsItemServiceRegistrationInvalid
        : public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, uint64_t, Category>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationRegistrationInvalid, ServiceIntegrationTestsItemServiceRegistrationInvalid,
        ::testing::Values(
                std::make_tuple("", "many words", 1, Category::beauty),
                std::make_tuple("   ", "07-05-2004", 10, Category::clothes),
                std::make_tuple("   ", " ", 10, Category::clothes),
                std::make_tuple("Makar", "04-05-1070", 0, Category::unknown))
);

TEST_P(ServiceIntegrationTestsItemServiceRegistrationInvalid, ItemServiceIntegrationRegistrationInvalid) {
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, false);
}


INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationDeleteReqestId, ServiceIntegrationTestsItemServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceIntegrationTestsItemServiceRequestId, ItemServiceIntegrationDeleteReqestId) {
    auto const& id = GetParam();
    auto request = DeleteItemReq{.requestId = id, .itemId = 0, .count = 1};
    auto responce = itemService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceIntegrationTestsItemServiceDeleteValid
: public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationDeleteValid, ServiceIntegrationTestsItemServiceDeleteValid,
        ::testing::Values(
                std::make_tuple(0,1),
                std::make_tuple(1,1),
                std::make_tuple(2,3),
                std::make_tuple(3,5)
                ));

TEST_P(ServiceIntegrationTestsItemServiceDeleteValid, ItemServiceIntegrationDeleteValid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceIntegrationTestsItemServiceDeleteInvalid
        : public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationDeleteInvalid, ServiceIntegrationTestsItemServiceDeleteInvalid,
        ::testing::Values(
                std::make_tuple(0,0),
                std::make_tuple(4,1),
                std::make_tuple(5,0),
                std::make_tuple(6,5)
        ));

TEST_P(ServiceIntegrationTestsItemServiceDeleteInvalid, ItemServiceIntegrationDeleteInvalid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, false);
}

class ServiceIntegrationTestsItemServiceDeleteChangeLenValid
        : public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t> >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationDeleteChangeLenValid, ServiceIntegrationTestsItemServiceDeleteChangeLenValid,
        ::testing::Values(
                std::make_tuple(0,5),
                std::make_tuple(1,5),
                std::make_tuple(2,5),
                std::make_tuple(3,5)
        ));

TEST_P(ServiceIntegrationTestsItemServiceDeleteChangeLenValid, ItemServiceIntegrationDeleteChangeLenValid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    auto responce = itemService.process(DeleteItemReq{.requestId = 0, .itemId = id, .count = n});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(responce.success, true);
    ASSERT_EQ(n2, n1-1);
}

class ServiceIntegrationTestsItemServiceDeleteChangeLenInvalid
        : public  ItemIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t> >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceIntegrationDeleteChangeLenInvalid, ServiceIntegrationTestsItemServiceDeleteChangeLenInvalid,
        ::testing::Values(
                std::make_tuple(0,0),
                std::make_tuple(0,1),
                std::make_tuple(1,4),
                std::make_tuple(4,1),
                std::make_tuple(5,0),
                std::make_tuple(6,5)
        ));

TEST_P(ServiceIntegrationTestsItemServiceDeleteChangeLenInvalid, ItemServiceIntegrationDeleteChangeLenInvalid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    auto responce = itemService.process(DeleteItemReq{.requestId = 0, .itemId = id, .count = n});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n2, n1);
}

TEST_F( ItemIntegrationServiceTests, GetAllTest) {
    addItems();
    auto n = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n, 4);
}

TEST_F( ItemIntegrationServiceTests, GetAllTestWithAdding) {
    addItems();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n2, n1+3);
}

