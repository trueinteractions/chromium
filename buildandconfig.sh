#!/bin/sh
export PATH=$PATH:/Users/trevorlinton/Documents/Create/create-repo-mac/depot_tools
export GYP_GENERATORS=ninja
cd /Users/trevorlinton/Documents/Create/create-repo-mac/src/
./build/gyp_chromium -Denable_themes=0 -Denable_plugins=0 -Denable_settings_app=0 -Denable_plugin_installation=0 -Denable_background=0 -Denable_spellcheck=0 -Ddisable_nacl=1 -Ddisable_pnacl=1 -Denable_task_manager=0 -Djava_bridge=0 -Duse_system_zlib=1 -Dgoogle_tv=0 -Dchromeos=0 -Dfastbuild=1 -Ddcheck_always_on=0 -Dlogging_like_official_build=0 -Dtracing_like_official_build=0 -Denable_extensions=0 -Denable_google_now=0 -Denable_app_list=0 -Denable_settings_app=0 -Denable_managed_users=0 -Denable_enhanced_bookmarks=0 -Duse_ash=0 -Duse_aura=0 -Dclang_use_chrome_plugins=0 -Dnacl_untrusted_build=0 -Duse_brlapi=0 -Dinternal_pdf=0 -Denable_one_click_signin=0 -Dsafe_browsing=0 --no-circular-check content/content.gyp
ninja -C out/Release nw
