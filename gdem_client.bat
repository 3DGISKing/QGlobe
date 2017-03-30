cd gdem_common
del gdem_config.h
copy gdem_config.h_org gdem_config.h

cd ../gdem_client
del gdemlib.lib
copy gdemlib.lib_org gdemlib.lib
cd ..

pause