// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@class InfoBarContainerController;
class InfoBarDelegate;

// A controller for an infobar in the browser window.  There is one
// controller per infobar view.  The base InfoBarController is able to
// draw an icon, a text message, and a close button.  Subclasses can
// override addAdditionalControls to customize the UI.
@interface InfoBarController : NSViewController {
 @private
  InfoBarContainerController* containerController_;  // weak, owns us

 @protected
  InfoBarDelegate* delegate_;  // weak
  IBOutlet NSImageView* image_;
  IBOutlet NSTextField* label_;
  IBOutlet NSButton* closeButton_;
};

// Initializes a new InfoBarController.
- (id)initWithDelegate:(InfoBarDelegate*)delegate;

// Dismisses the infobar without taking any action.
- (IBAction)dismiss:(id)sender;

// Subclasses can override this method to add additional controls to
// the infobar view.  This method is called by awakeFromNib.  The
// default implementation does nothing.
- (void)addAdditionalControls;

@property(assign, nonatomic) InfoBarContainerController* containerController;
@property(readonly) InfoBarDelegate* delegate;

@end

/////////////////////////////////////////////////////////////////////////
// InfoBarController subclasses, one for each InfoBarDelegate
// subclass.  Each of these subclasses overrides addAdditionalControls to
// configure its view as necessary.

@interface AlertInfoBarController : InfoBarController {
}
@end


@interface LinkInfoBarController : InfoBarController {
}
// Called when there is a click on the link in the infobar.
- (void)linkClicked;
@end


@interface ConfirmInfoBarController : InfoBarController {
}
// Called when the ok and cancel buttons are clicked.
- (IBAction)ok:(id)sender;
- (IBAction)cancel:(id)sender;
@end
