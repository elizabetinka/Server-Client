add_library(BusinessLayer
        ClientService.cpp
        ItemService.cpp
       ClientService.h
        ItemService.h
        Defs/ClientDefs.h
        Defs/CommonDefs.h
        Defs/ItemDefs.h
)

target_link_libraries(BusinessLayer PRIVATE DataLayer Utils)