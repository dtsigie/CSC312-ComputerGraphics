#include "../csc321.h"
#include "../sceneview/MyScene.h"

bool MyScene::selectNode(int x, int y) {
    if (!isLoaded)
        return false;
    // ToDo: pick which node is under the given pixel
    // by tracing a ray into the world
    return false;
}

void MyScene::moveSelectedNode( const Matrix4 &mat )
{
    // ToDo: Move the selected node
}

