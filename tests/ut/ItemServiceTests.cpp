//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../../src/Service/ItemService.h"
#include "../mock/MockItemRepository.h"
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Return;

class ItemServiceTests : public testing::Test {

protected:
    std::shared_ptr<MockItemRepository> itemRepository = std::make_shared<MockItemRepository>();
    ItemService itemService{itemRepository};

public:
    virtual ~ItemServiceTests() override = default;
};

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
    EXPECT_CALL(*itemRepository, Add(_)).Times(1);
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    itemService.process(request);
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
    EXPECT_CALL(*itemRepository, Add(_)).Times(0);
    auto const& [name, desc, n, cat] = GetParam();
    auto request = AddItemReq{.requestId = 0, .name = name, .description = desc, .count = n, .category = cat};
    itemService.process(request);
}

class ServiceTestsItemServiceDeleteValid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteValid, ServiceTestsItemServiceDeleteValid,
        ::testing::Values(
                std::make_tuple(0,5),
                std::make_tuple(1,5),
                std::make_tuple(2,5),
                std::make_tuple(3,5)
        ));

TEST_P(ServiceTestsItemServiceDeleteValid, ItemServiceDeleteValid) {

    EXPECT_CALL(*itemRepository, getById(_)).WillOnce(Return(std::make_pair(true,ItemInfo{.item = Item(), .count = 5})));
    EXPECT_CALL(*itemRepository, Delete(_)).Times(1);

    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    itemService.process(request);
}

class ServiceTestsItemServiceDeleteModifyValid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteModifyValid, ServiceTestsItemServiceDeleteModifyValid,
        ::testing::Values(
                std::make_tuple(0,1),
                std::make_tuple(1,2),
                std::make_tuple(2,3),
                std::make_tuple(3,4)
        ));

TEST_P(ServiceTestsItemServiceDeleteModifyValid, ItemServiceDeleteModifyValid) {

    EXPECT_CALL(*itemRepository, getById(_)).WillOnce(Return(std::make_pair(true,ItemInfo{.item = Item(), .count = 5})));
    EXPECT_CALL(*itemRepository, Delete(_)).Times(0);
    EXPECT_CALL(*itemRepository, Modify(_)).Times(1);

    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    itemService.process(request);
}

class ServiceTestsItemServiceDeleteInvalid
        : public  ItemServiceTests, public ::testing::WithParamInterface<std::tuple<uint64_t,uint64_t>>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceDeleteInvalid, ServiceTestsItemServiceDeleteInvalid,
        ::testing::Values(
                std::make_tuple(0,0),
                std::make_tuple(1,5),
                std::make_tuple(2,0),
                std::make_tuple(3,5)
        ));

TEST_P(ServiceTestsItemServiceDeleteInvalid, ItemServiceDeleteInvalid) {

    EXPECT_CALL(*itemRepository, getById(_)).WillRepeatedly(Return(std::make_pair(false,ItemInfo{.item = Item(), .count = 5})));
    EXPECT_CALL(*itemRepository, Delete(_)).Times(0);

    auto const& [id,n] = GetParam();
    auto request = DeleteItemReq{.requestId = 0, .itemId = id, .count = n};
    itemService.process(request);
}

TEST_F( ItemServiceTests, GetAllTest) {
    EXPECT_CALL(*itemRepository, getAll()).Times(1);
    itemService.process(GetAllItemReq{.requestId = 0});
}

class ServiceTestsItemModifyValid
        : public ItemServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, Category, uint64_t >>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceModifyValid, ServiceTestsItemModifyValid,
        ::testing::Values(
                std::make_tuple("name", "description",Category::unknown, 10),
                std::make_tuple("Лиза", "07-05-2004",Category::clothes, 5),
                std::make_tuple("Л", "      ",Category::technic,2),
                std::make_tuple("Makar", "",Category::beauty, 3)
        ));

TEST_P(ServiceTestsItemModifyValid, ItemServiceModifyValid) {
    EXPECT_CALL(*itemRepository, Modify(_)).Times(1);

    auto const& [name, description, cat, count] = GetParam();
    auto request = ItemModifyReq{.requestId = 0, .new_item = ItemInfo{.item = Item(name,description,cat), .count = count}};
    itemService.process(request);
}

class ServiceTestsItemModifyInvalid
        : public ItemServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string, Category, uint64_t >>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemServiceModifyInvalid, ServiceTestsItemModifyInvalid,
        ::testing::Values(
                std::make_tuple("", "description",Category::unknown, 10),
                std::make_tuple("   ", "07-05-2004",Category::clothes, 5),
                std::make_tuple("Л", "      ",Category::technic,0),
                std::make_tuple("   ", "",Category::beauty, 0)
        ));

TEST_P(ServiceTestsItemModifyInvalid, ItemServiceModifyInvalid) {
    EXPECT_CALL(*itemRepository, Modify(_)).Times(0);

    auto const& [name, description, cat, count] = GetParam();
    auto request = ItemModifyReq{.requestId = 0, .new_item = ItemInfo{.item = Item(name,description,cat), .count = count}};
    itemService.process(request);
}
