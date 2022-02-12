#!/bin/sh
WHICH_DIR=~/Desktop/editor_upload
rm -rf $WHICH_DIR
test "$1" = clean && exit 0
cp -R distro $WHICH_DIR
cp -r distro_aux/. $WHICH_DIR
cd $WHICH_DIR
git init
git config user.name thebetioplane
git config user.email thebetioplane@users.noreply.github.com
git remote add origin git@github.com:thebetioplane/osuReplayEditorV3.git
git add --all
git commit -m "`date '+%Y-%m-%d [%y%j-%H]'`"
git tag beta
git push -uf origin master
git push -f origin beta
rm -rf $WHICH_DIR
