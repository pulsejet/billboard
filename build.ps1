rm a.exe -erroraction 'silentlycontinue'
g++ -LSMFL/lib -Lcurl/lib -Iinclude src/main.cpp src/painter.cpp src/scene/event.cpp src/data.cpp src/transforms.cpp src/models.cpp src/http.cpp src/common.cpp -lsfml-graphics -lsfml-window -lsfml-system -lcurl
./a.exe -erroraction 'silentlycontinue'
