// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/menu_model.h"
#include "ui/base/models/menu_model_delegate.h"
#include "views/controls/menu/menu_item_view.h"
#include "views/controls/menu/menu_model_adapter.h"
#include "views/controls/menu/submenu_view.h"
#include "views/test/views_test_base.h"

namespace {

// Base command id for test menu and its submenu.
const int kRootIdBase = 100;
const int kSubmenuIdBase = 200;

// Offset to return for GetFirstItemIndex().  This is an arbitrary
// number to ensure that we aren't assuming it is 0.
const int kFirstItemIndex = 25;

class MenuModelBase : public ui::MenuModel {
 public:
  MenuModelBase(int command_id_base) : command_id_base_(command_id_base),
                                       last_activation_(-1) {
  }

  virtual ~MenuModelBase() {
  }

  // ui::MenuModel implementation:

  virtual bool HasIcons() const OVERRIDE {
    return false;
  }

  virtual int GetFirstItemIndex(gfx::NativeMenu native_menu) const OVERRIDE {
    return kFirstItemIndex;
  }

  virtual int GetItemCount() const OVERRIDE {
    return static_cast<int>(items_.size());
  }

  virtual ItemType GetTypeAt(int index) const OVERRIDE {
    return items_[index - GetFirstItemIndex(NULL)].type;
  }

  virtual int GetCommandIdAt(int index) const OVERRIDE {
    return index - GetFirstItemIndex(NULL) + command_id_base_;
  }

  string16 GetLabelAt(int index) const OVERRIDE {
    return items_[index - GetFirstItemIndex(NULL)].label;
  }

  virtual bool IsItemDynamicAt(int index) const OVERRIDE {
    return false;
  }

  virtual const gfx::Font* GetLabelFontAt(int index) const OVERRIDE {
    return NULL;
  }

  virtual bool GetAcceleratorAt(int index,
                                ui::Accelerator* accelerator) const OVERRIDE {
    return false;
  }

  virtual bool IsItemCheckedAt(int index) const OVERRIDE {
    return false;
  }

  virtual int GetGroupIdAt(int index) const OVERRIDE {
    return 0;
  }

  virtual bool GetIconAt(int index, SkBitmap* icon) OVERRIDE {
    return false;
  }

  virtual ui::ButtonMenuItemModel* GetButtonMenuItemAt(
      int index) const OVERRIDE {
    return NULL;
  }

  virtual bool IsEnabledAt(int index) const OVERRIDE {
    return true;
  }

  virtual bool IsVisibleAt(int index) const OVERRIDE {
    return true;
  }

  virtual MenuModel* GetSubmenuModelAt(int index) const OVERRIDE {
    return items_[index - GetFirstItemIndex(NULL)].submenu;
  }

  virtual void HighlightChangedTo(int index) OVERRIDE {
  }

  virtual void ActivatedAt(int index) OVERRIDE {
    set_last_activation(index);
  }

  virtual void ActivatedAtWithDisposition(int index, int disposition) OVERRIDE {
    ActivatedAt(index);
  }

  virtual void MenuWillShow() OVERRIDE {
  }

  virtual void MenuClosed() OVERRIDE {
  }

  virtual void SetMenuModelDelegate(
      ui::MenuModelDelegate* delegate) OVERRIDE {
  }

  // Item definition.
  struct Item {
    Item(ItemType item_type,
         const std::string& item_label,
         ui::MenuModel* item_submenu)
        : type(item_type),
          label(ASCIIToUTF16(item_label)),
          submenu(item_submenu) {
    }

    ItemType type;
    string16 label;
    ui::MenuModel* submenu;
  };

  const Item& GetItemDefinition(int index) {
    return items_[index];
  }

  // Access index argument to ActivatedAt() or ActivatedAtWithDisposition().
  int last_activation() const { return last_activation_; }
  void set_last_activation(int last_activation) {
    last_activation_ = last_activation;
  }

 protected:
  std::vector<Item> items_;

 private:
  int command_id_base_;
  int last_activation_;

  DISALLOW_COPY_AND_ASSIGN(MenuModelBase);
};

class SubmenuModel : public MenuModelBase {
 public:
  SubmenuModel() : MenuModelBase(kSubmenuIdBase) {
    items_.push_back(Item(TYPE_COMMAND, "submenu item 0", NULL));
    items_.push_back(Item(TYPE_COMMAND, "submenu item 1", NULL));
  }

  virtual ~SubmenuModel() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SubmenuModel);
};

class RootModel : public MenuModelBase {
 public:
  RootModel() : MenuModelBase(kRootIdBase) {
    submenu_model_.reset(new SubmenuModel);

    items_.push_back(Item(TYPE_COMMAND, "command 0", NULL));
    items_.push_back(Item(TYPE_CHECK, "check 1", NULL));
    items_.push_back(Item(TYPE_SEPARATOR, "", NULL));
    items_.push_back(Item(TYPE_SUBMENU, "submenu 3", submenu_model_.get()));
    items_.push_back(Item(TYPE_RADIO, "radio 4", NULL));
  }

  virtual ~RootModel() {
  }

 private:
  scoped_ptr<MenuModel> submenu_model_;

  DISALLOW_COPY_AND_ASSIGN(RootModel);
};

}  // namespace

namespace views {

typedef ViewsTestBase MenuModelAdapterTest;

TEST_F(MenuModelAdapterTest, BasicTest) {
  // Build model and adapter.
  RootModel model;
  views::MenuModelAdapter delegate(&model);

  // Create menu.  Build menu twice to check that rebuilding works properly.
  scoped_ptr<views::MenuItemView> menu(new views::MenuItemView(&delegate));
  delegate.BuildMenu(menu.get());
  delegate.BuildMenu(menu.get());
  EXPECT_TRUE(menu->HasSubmenu());

  // Check top level menu items.
  views::SubmenuView* item_container = menu->GetSubmenu();
  EXPECT_EQ(5, item_container->child_count());

  for (int i = 0; i < item_container->child_count(); ++i) {
    const MenuModelBase::Item& model_item = model.GetItemDefinition(i);

    const int id = i + kRootIdBase;
    MenuItemView* item = menu->GetMenuItemByID(id);
    if (!item) {
      EXPECT_EQ(ui::MenuModel::TYPE_SEPARATOR, model_item.type);
      continue;
    }

    // Check placement.
    EXPECT_EQ(i, menu->GetSubmenu()->GetIndexOf(item));

    // Check type.
    switch (model_item.type) {
      case ui::MenuModel::TYPE_COMMAND:
        EXPECT_EQ(views::MenuItemView::NORMAL, item->GetType());
        break;
      case ui::MenuModel::TYPE_CHECK:
        EXPECT_EQ(views::MenuItemView::CHECKBOX, item->GetType());
        break;
      case ui::MenuModel::TYPE_RADIO:
        EXPECT_EQ(views::MenuItemView::RADIO, item->GetType());
        break;
      case ui::MenuModel::TYPE_SEPARATOR:
      case ui::MenuModel::TYPE_BUTTON_ITEM:
        break;
      case ui::MenuModel::TYPE_SUBMENU:
        EXPECT_EQ(views::MenuItemView::SUBMENU, item->GetType());
        break;
    }

    // Check activation.
    static_cast<views::MenuDelegate*>(&delegate)->ExecuteCommand(id);
    EXPECT_EQ(i + kFirstItemIndex, model.last_activation());
    model.set_last_activation(-1);
  }

  // Check submenu items.
  views::MenuItemView* submenu = menu->GetMenuItemByID(103);
  views::SubmenuView* subitem_container = submenu->GetSubmenu();
  EXPECT_EQ(2, subitem_container->child_count());

  for (int i = 0; i < subitem_container->child_count(); ++i) {
    MenuModelBase* submodel = static_cast<MenuModelBase*>(
        model.GetSubmenuModelAt(3 + kFirstItemIndex));
    EXPECT_TRUE(submodel);

    const MenuModelBase::Item& model_item = submodel->GetItemDefinition(i);

    const int id = i + kSubmenuIdBase;
    MenuItemView* item = menu->GetMenuItemByID(id);
    if (!item) {
      EXPECT_EQ(ui::MenuModel::TYPE_SEPARATOR, model_item.type);
      continue;
    }

    // Check placement.
    EXPECT_EQ(i, submenu->GetSubmenu()->GetIndexOf(item));

    // Check type.
    switch (model_item.type) {
      case ui::MenuModel::TYPE_COMMAND:
        EXPECT_EQ(views::MenuItemView::NORMAL, item->GetType());
        break;
      case ui::MenuModel::TYPE_CHECK:
        EXPECT_EQ(views::MenuItemView::CHECKBOX, item->GetType());
        break;
      case ui::MenuModel::TYPE_RADIO:
        EXPECT_EQ(views::MenuItemView::RADIO, item->GetType());
        break;
      case ui::MenuModel::TYPE_SEPARATOR:
      case ui::MenuModel::TYPE_BUTTON_ITEM:
        break;
      case ui::MenuModel::TYPE_SUBMENU:
        EXPECT_EQ(views::MenuItemView::SUBMENU, item->GetType());
        break;
    }

    // Check activation.
    static_cast<views::MenuDelegate*>(&delegate)->ExecuteCommand(id);
    EXPECT_EQ(i + kFirstItemIndex, submodel->last_activation());
    submodel->set_last_activation(-1);
  }

  // Check that selecting the root item is safe.  The MenuModel does
  // not care about the root so MenuModelAdapter should do nothing
  // (not hit the NOTREACHED check) when the root is selected.
  static_cast<views::MenuDelegate*>(&delegate)->SelectionChanged(menu.get());
}

}  // namespace views
