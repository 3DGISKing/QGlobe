cd gdem_common
del /S /F /Q gdem_config.h
copy gdem_config.h_alone gdem_config.h

cd ../gdem_client
del gdemlib.lib
copy gdemlib.lib_alone gdemlib.lib
cd ..

pause