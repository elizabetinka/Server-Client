//
// Created by e.kravchenkova on 27.03.2025.
//

#include "../../src/Model/Repository/ItemRepository.h"
#include <gtest/gtest.h>

class ItemRepositoryTests : public testing::Test {

public:
    virtual ~ItemRepositoryTests() override {
        itemRepository.DeleteAll();
    }

protected:
    ItemRepository itemRepository;

    void addItems(){
        itemRepository.Add(ItemInfo(Item("name", "description", Category::clothes), 10));
        itemRepository.Add(ItemInfo(Item("name", "description", Category::clothes), 10));
        itemRepository.Add(ItemInfo(Item("name", "description", Category::clothes), 10));
        itemRepository.Add(ItemInfo(Item("name", "description", Category::clothes), 10));
    }
};

class ItemRepositoryAddTestsValid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<ItemInfo>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryAddTestsValidTest, ItemRepositoryAddTestsValid,
        ::testing::Values(
                ItemInfo(Item("name", "description", Category::unknown), 10),
                ItemInfo(Item("name", "", Category::clothes), 10),
                ItemInfo(Item("", "description", Category::clothes), 10),
                ItemInfo(Item("", "", Category::clothes), 10),
                ItemInfo(Item("name", "description", Category::clothes), 0))
        );

TEST_P(ItemRepositoryAddTestsValid, CItemRepositoryAddTestsValidTest) {
    auto const& Item = GetParam();
    auto responce = itemRepository.Add(Item);
    ASSERT_EQ(responce.first, true);
}


class ItemRepositoryAddTestsInvalid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<ItemInfo>
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryAddTestsInvalidTest, ItemRepositoryAddTestsInvalid,
        ::testing::Values(
                ItemInfo(Item("name", "description", Category::unknown), 10),
                ItemInfo(Item("name", "", Category::clothes), 10),
                ItemInfo(Item("", "description", Category::clothes), 10),
                ItemInfo(Item("", "", Category::clothes), 10),
                ItemInfo(Item("name", "description", Category::clothes), 0))
        );

TEST_P(ItemRepositoryAddTestsInvalid, ItemRepositoryAddTestsInvalidTest) {
    addItems();
    auto Item = GetParam();
    Item.item.id = 0;
    auto responce = itemRepository.Add(Item);
    ASSERT_EQ(responce.first, false);
}

class ItemRepositoryDeleteTestsValid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryDeleteTestsValidTest, ItemRepositoryDeleteTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ItemRepositoryDeleteTestsValid, CItemRepositoryDeleteTestsValidTest) {
    addItems();
    auto const& id = GetParam();
    auto responce = itemRepository.Delete(id);
    ASSERT_EQ(responce, true);
}


class ItemRepositoryDeleteTestsInvalid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryDeleteTestsInvalidTest, ItemRepositoryDeleteTestsInvalid,
        ::testing::Values(4,5,6,7,8));

TEST_P(ItemRepositoryDeleteTestsInvalid, ItemRepositoryDeleteTestsInvalidTest) {
    addItems();
    auto const& id = GetParam();
    auto responce = itemRepository.Delete(id);
    ASSERT_EQ(responce, false);
}

class ItemRepositoryModifyTestsValid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryModifyTestsValidTest, ItemRepositoryModifyTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ItemRepositoryModifyTestsValid, ItemRepositoryModifyTestsValidTest) {
    addItems();
    auto const& id = GetParam();
    std::string expected_name = "Liza";

    auto responce1 = itemRepository.getById(id);
    ASSERT_EQ(responce1.first, true);
    responce1.second.item.name = expected_name;

    auto responce = itemRepository.Modify(responce1.second);
    ASSERT_EQ(responce, true);

    auto responce3 = itemRepository.getById(id);
    ASSERT_EQ(responce3.first, true);
    ASSERT_EQ(responce3.second.item.name, expected_name);
}


class ItemRepositoryModifyTestsInvalid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryModifyTestsInvalidTest, ItemRepositoryModifyTestsInvalid,
        ::testing::Values(4,5,6,7,8));

TEST_P(ItemRepositoryModifyTestsInvalid, ItemRepositoryModifyTestsInvalidTest) {
    addItems();
    auto const& id = GetParam();
    std::string expected_name = "Liza";

    auto responce1 = itemRepository.getById(id);
    ASSERT_EQ(responce1.first, false);

    responce1.second.item.name = expected_name;
    auto responce = itemRepository.Modify(responce1.second);
    ASSERT_EQ(responce, false);

    auto responce3 = itemRepository.getById(id);
    ASSERT_EQ(responce3.first, false);
    ASSERT_NE(responce3.second.item.name, expected_name);
}


class ItemRepositoryGetByIdTestsValid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryGetByIdTestValid, ItemRepositoryGetByIdTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ItemRepositoryGetByIdTestsValid, ItemRepositoryGetByIdTestValid) {
    addItems();
    auto const& id = GetParam();

    auto responce1 = itemRepository.getById(id);
    ASSERT_EQ(responce1.first, true);
    ASSERT_EQ(responce1.second.item.id, id);
}

class ItemRepositoryGetByIdTestsInvalid
        : public  ItemRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ItemRepositoryGetByIdTestInvalid, ItemRepositoryGetByIdTestsInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ItemRepositoryGetByIdTestsInvalid, ItemRepositoryGetByIdTestInvalid) {
    addItems();
    auto const& id = GetParam();

    auto responce1 = itemRepository.getById(id);
    ASSERT_EQ(responce1.first, false);
    ASSERT_EQ(responce1.second.item.id, undefId);
}

TEST_F(ItemRepositoryTests, ItemRepositoryGetAllTest) {
    addItems();
    auto n = itemRepository.getAll().size();
    ASSERT_EQ(n,4);

    itemRepository.Add(ItemInfo(Item("name", "description", Category::unknown), 10));

    n = itemRepository.getAll().size();
    ASSERT_EQ(n,5);

    itemRepository.Delete(0);

    n = itemRepository.getAll().size();
    ASSERT_EQ(n,4);
}

TEST_F(ItemRepositoryTests, ItemRepositoryDeleteAllTest) {
    addItems();
    auto n = itemRepository.DeleteAll();
    ASSERT_EQ(n,4);

    n = itemRepository.getAll().size();
    ASSERT_EQ(n,0);
}
