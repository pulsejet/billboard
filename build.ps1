rm a.exe -erroraction 'silentlycontinue'
g++ -LSMFL/lib -Lcurl/lib -ISMFL/include -Icurl/include main.cpp Painter.cpp DataHandler.cpp -Iinclude -lsfml-graphics -lsfml-window -lsfml-system -lcurl
./a.exe -erroraction 'silentlycontinue'
