//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../src/Service/ItemService.h"

class ItemServiceTests : public testing::Test {

protected:
    ItemService itemService;

    void addItems(){
        itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "b", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "c", .description = "07-05-2004", .count = 5, .category = Category::beauty});
        itemService.process(AddItemReq{.requestId = 0, .name = "d", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    }

public:
    virtual ~ItemServiceTests() override {
        itemService.process(ItemDeleteAllReq{});
    }
};

class ServiceTestsItemServiceRequestId
        : public ItemServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceRegistrationReqestId, ServiceTestsItemServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceTestsItemServiceRequestId, ItemServiceRegistrationReqestId) {
    auto const& id = GetParam();
    auto request = AddItemReq{.requestId = id, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty};
    auto responce = itemService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceTestsItemServiceRegistrationValid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, uint64_t, Category>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceRegistrationValid, ServiceTestsItemServiceRegistrationValid,
        ::testing::Values(
                std::make_tuple("My name is Liza", "many words", 1, Category::beauty),
                std::make_tuple("Liza", "desc", 10, Category::technic),
                std::make_tuple("Л", "07-05-2004", 10, Category::clothes),
                std::make_tuple("Liza", "", 10, Category::technic),
                std::make_tuple("Makar", "   ", 10, Category::unknown),
                std::make_tuple("Makar", "04-05-1070", 10, Category::unknown))
                );

TEST_P(ServiceTestsItemServiceRegistrationValid, ItemServiceRegistrationValid) {
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceTestsItemServiceRegistrationInvalid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, uint64_t, Category>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceRegistrationInvalid, ServiceTestsItemServiceRegistrationInvalid,
        ::testing::Values(
                std::make_tuple("", "many words", 1, Category::beauty),
                std::make_tuple("   ", "07-05-2004", 10, Category::clothes),
                std::make_tuple("   ", " ", 10, Category::clothes),
                std::make_tuple("Makar", "04-05-1070", 0, Category::unknown))
);

TEST_P(ServiceTestsItemServiceRegistrationInvalid, ItemServiceRegistrationInvalid) {
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, false);
}


INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteReqestId, ServiceTestsItemServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceTestsItemServiceRequestId, ItemServiceDeleteReqestId) {
    auto const& id = GetParam();
    auto request = DeleteItemReq{.requestId = id, .itemId = 0, .count = 1};
    auto responce = itemService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceTestsItemServiceDeleteValid
: public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteValid, ServiceTestsItemServiceDeleteValid,
        ::testing::Values(
                std::make_tuple(0,1),
                std::make_tuple(1,1),
                std::make_tuple(2,3),
                std::make_tuple(3,5)
                ));

TEST_P(ServiceTestsItemServiceDeleteValid, ItemServiceDeleteValid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceTestsItemServiceDeleteInvalid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteInvalid, ServiceTestsItemServiceDeleteInvalid,
        ::testing::Values(
                std::make_tuple(0,0),
                std::make_tuple(4,1),
                std::make_tuple(5,0),
                std::make_tuple(6,5)
        ));

TEST_P(ServiceTestsItemServiceDeleteInvalid, ItemServiceDeleteInvalid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    auto responce = itemService.process(request);
    ASSERT_EQ(responce.success, false);
}

class ServiceTestsItemServiceDeleteChangeLenValid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t> >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteChangeLenValid, ServiceTestsItemServiceDeleteChangeLenValid,
        ::testing::Values(
                std::make_tuple(0,5),
                std::make_tuple(1,5),
                std::make_tuple(2,5),
                std::make_tuple(3,5)
        ));

TEST_P(ServiceTestsItemServiceDeleteChangeLenValid, ItemServiceDeleteChangeLenValid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    auto responce = itemService.process(DeleteItemReq{.requestId = 0, .itemId = id, .count = n});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(responce.success, true);
    ASSERT_EQ(n2, n1-1);
}

class ServiceTestsItemServiceDeleteChangeLenInvalid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t> >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteChangeLenInvalid, ServiceTestsItemServiceDeleteChangeLenInvalid,
        ::testing::Values(
                std::make_tuple(0,0),
                std::make_tuple(0,1),
                std::make_tuple(1,4),
                std::make_tuple(4,1),
                std::make_tuple(5,0),
                std::make_tuple(6,5)
        ));

TEST_P(ServiceTestsItemServiceDeleteChangeLenInvalid, ItemServiceDeleteChangeLenInvalid) {
    addItems();
    auto const& [id,n] = GetParam();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    auto responce = itemService.process(DeleteItemReq{.requestId = 0, .itemId = id, .count = n});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n2, n1);
}

TEST_F( ItemServiceTests, GetAllTest) {
    addItems();
    auto n = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n, 4);
}

TEST_F( ItemServiceTests, GetAllTestWithAdding) {
    addItems();
    auto n1 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    itemService.process(AddItemReq{.requestId = 0, .name = "a", .description = "07-05-2004", .count = 5, .category = Category::beauty});
    auto n2 = itemService.process(GetAllItemReq{.requestId = 0}).items.size();
    ASSERT_EQ(n2, n1+3);
}

