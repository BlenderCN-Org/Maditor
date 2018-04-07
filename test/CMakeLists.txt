
project(Maditor_Test)


set(sources "")
FolderSources(sources "" "" integrationtests.cpp)
FolderSources(sources "" mocks dialogmanagermock.h)

MESSAGE (STATUS ${sources})

add_gtestmock(Maditor_Test ${sources})

target_link_libraries(Maditor_Test PRIVATE Maditor_Model)
