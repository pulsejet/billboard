rm a.exe -erroraction 'silentlycontinue'
g++ -LSMFL/lib -Lcurl/lib -Iinclude main.cpp Painter.cpp DataHandler.cpp -lsfml-graphics -lsfml-window -lsfml-system -lcurl
./a.exe -erroraction 'silentlycontinue'
