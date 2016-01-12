#include "../csc321.h"
#include "MyScene.h"
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <cmath>

// string constants used for parsing
// feel free to add any new keywords that you think may be helpfull
const std::string TOKEN_BACKGROUND     = "background";
const std::string TOKEN_COLOR          = "color";
const std::string TOKEN_OB             = "[";
const std::string TOKEN_CB             = "]";
const std::string TOKEN_CAMERA         = "camera";
const std::string TOKEN_EYE            = "eye";
const std::string TOKEN_LOOK           = "look";
const std::string TOKEN_FOCUS          = "focus";
const std::string TOKEN_UP             = "up";
const std::string TOKEN_ANGLE          = "angle";
const std::string TOKEN_NEAR_FAR       = "near-far";
const std::string TOKEN_LIGHT          = "light";
const std::string TOKEN_LIGHTTYPE      = "type";
const std::string TOKEN_POINT          = "point";
const std::string TOKEN_DIRECTIONAL    = "directional";
const std::string TOKEN_SPOTLIGHT      = "spotlight";
const std::string TOKEN_POSITION       = "position";
const std::string TOKEN_DIRECTION      = "direction";
const std::string TOKEN_FUNCTION       = "function";
const std::string TOKEN_APERTURE       = "aperture";
const std::string TOKEN_EXPONENT       = "exponent";
const std::string TOKEN_MASTERSUBGRAPH = "mastersubgraph";
const std::string TOKEN_TRANS          = "trans";
const std::string TOKEN_ROTATE         = "rotate";
const std::string TOKEN_TRANSLATE      = "translate";
const std::string TOKEN_SCALE          = "scale";
const std::string TOKEN_MATRIXRC       = "matrixRC";
const std::string TOKEN_MATRIXCR       = "matrixCR";
const std::string TOKEN_OBJECT         = "object";
const std::string TOKEN_CUBE           = "cube";
const std::string TOKEN_CYLINDER       = "cylinder";
const std::string TOKEN_CONE           = "cone";
const std::string TOKEN_SPHERE         = "sphere";
const std::string TOKEN_COW            = "cow";
const std::string TOKEN_AMBIENT        = "ambient";
const std::string TOKEN_DIFFUSE        = "diffuse";
const std::string TOKEN_SPECULAR       = "specular";
const std::string TOKEN_REFLECT        = "reflect";
const std::string TOKEN_TRANSPARENT    = "transparent";
const std::string TOKEN_EMIT           = "emit";
const std::string TOKEN_SHINE          = "shine";
const std::string TOKEN_IOR            = "ior";
const std::string TOKEN_TEXTURE        = "texture";
const std::string TOKEN_SUBGRAPH       = "subgraph";

MyScene::MyScene() :
progress(1.0),
bDoRender(false),
cube(1),
cylinder(32,1),
cone(32,1),
sphere(5),
ambientLight(0.2,0.2,0.2)
{
    // initialize your variables here
	masters = 0;
    //
    resetScene();
}

MyScene::~MyScene() {
    // destroy your variables here
	if (masters != 0)
		delete masters;
}

void MyScene::resetScene() {
    // reset scene to defaults (empty the scene tree)
    background = Color();
    camera.setFrom(Point3(0, 0, 1));
    camera.setLook(Vector3(0, 0, -1));
    camera.setUp(Vector3(0, 1, 0));
    camera.setZoom(60.0);
    camera.setNearFar(0.01, 100.0);
    camera.setProjectionCenter( Point3(0,0,0) );
    camera.setSkew(0.0);
    camera.setAspectRatioScale(1.0);

    lights.clear();

    ambientLight = Color(0,0,0);

    // reset your data here
	if (masters != 0)
		delete masters;
	masters = new MasterSubgraphs();
	isLoaded = false;
}

bool MyScene::loadSceneFile(std::string filename) {
    // load the scenefile with the given filename
    // return true if the file was loaded succesfully
    resetScene();
    // parse file:
    Parser p(filename);
    bool done = false;
    errorMessage = "";
    do {
        done = p.nextToken();
        if (p.getToken() == TOKEN_BACKGROUND)
            done = !parseBackground(p);
        else if (p.getToken() == TOKEN_CAMERA)
            done = !parseCamera(p);
        else if (p.getToken() == TOKEN_LIGHT)
            done = !parseLight(p);
        else if (p.getToken() == TOKEN_MASTERSUBGRAPH)
            done = (parseMasterSubgraph(p) == 0);
        else if (!done) {
            errorMessage = "Unrecognized token at root level: \"" + p.getToken() + "\"";
            done = true;
        }
    } while (!done);
    // ToDo: check that there is a root mastersubgraph
    // and that no errors occured while loading
    isLoaded = (errorMessage.length() == 0) && (masters->get("root") != 0);
    if (isLoaded) {
        // perform any post-loading computations here
        // (such as flattening the tree or building
        // ray-acceleration structures)

    } else if (errorMessage.length() == 0)
        errorMessage = "Unable to locate root mastersubgraph";

    return isLoaded;
}

bool MyScene::parseBackground(Parser& p) {
    // parse the background block
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_COLOR) {
          p.nextToken(); background[0] = static_cast<float>(p.getValue());
          p.nextToken(); background[1] = static_cast<float>(p.getValue());
          p.nextToken(); background[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in background block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseCamera(Parser& p) {
    // parse the camera
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_EYE) {
            Point3 eye;
            p.nextToken(); eye[0] = p.getValue();
            p.nextToken(); eye[1] = p.getValue();
            p.nextToken(); eye[2] = p.getValue();
            camera.setFrom(eye);
        } else if (p.getToken() == TOKEN_LOOK) {
            Vector3 look;
            p.nextToken(); look[0] = p.getValue();
            p.nextToken(); look[1] = p.getValue();
            p.nextToken(); look[2] = p.getValue();
            camera.setLook(look);
        } else if (p.getToken() == TOKEN_FOCUS) {
            Point3 focus;
            p.nextToken(); focus[0] = p.getValue();
            p.nextToken(); focus[1] = p.getValue();
            p.nextToken(); focus[2] = p.getValue();
            camera.setAt(focus);
        } else if (p.getToken() == TOKEN_UP) {
            Vector3 up;
            p.nextToken(); up[0] = p.getValue();
            p.nextToken(); up[1] = p.getValue();
            p.nextToken(); up[2] = p.getValue();
            camera.setUp(up);
        } else if (p.getToken() == TOKEN_ANGLE) {
            p.nextToken(); camera.setZoom(p.getValue());
        } else if (p.getToken() == TOKEN_NEAR_FAR) {
            double n, f;
            p.nextToken(); n = p.getValue();
            p.nextToken(); f = p.getValue();
            camera.setNearFar(n, f);
        } else {
            errorMessage = "Unrecognized token in camera block: \"" + p.getToken() + "\"";
            return false;
        }
    } while (true);
    return true;
}

bool MyScene::parseLight(Parser& p) {
    // parse a light source block
    Light l;
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_LIGHTTYPE) {
            p.nextToken();
            if (p.getToken() == TOKEN_POINT)
                l.setType( Light::POINTLIGHT );
            else if (p.getToken() == TOKEN_DIRECTIONAL)
                l.setType( Light::DIRECTIONAL );
            else if (p.getToken() == TOKEN_SPOTLIGHT)
                l.setType( Light::SPOTLIGHT );
            else {
                errorMessage = "Unknown light type: \"" + p.getToken() + "\"";
                return false;
            }
        } else if (p.getToken() == TOKEN_POSITION) {
            Point3 pos;
            p.nextToken(); pos[0] = p.getValue();
            p.nextToken(); pos[1] = p.getValue();
            p.nextToken(); pos[2] = p.getValue();
            l.setPosition( pos );
        } else if (p.getToken() == TOKEN_COLOR) {
            Color color;
            p.nextToken(); color[0] = static_cast<float>(p.getValue());
            p.nextToken(); color[1] = static_cast<float>(p.getValue());
            p.nextToken(); color[2] = static_cast<float>(p.getValue());
            l.setColor( color );
        } else if (p.getToken() == TOKEN_FUNCTION) {
            float f0,f1,f2;
            p.nextToken(); f0 = static_cast<float>(p.getValue());
            p.nextToken(); f1 = static_cast<float>(p.getValue());
            p.nextToken(); f2 = static_cast<float>(p.getValue());
            l.setFalloff( f0, f1, f2 );
        } else if (p.getToken() == TOKEN_DIRECTION) {
            Vector3 dir;
            p.nextToken(); dir[0] = p.getValue();
            p.nextToken(); dir[1] = p.getValue();
            p.nextToken(); dir[2] = p.getValue();
            dir.normalize();
            l.setDirection( dir );
        } else if (p.getToken() == TOKEN_APERTURE) {
            p.nextToken(); l.setAperture( p.getValue() );
        } else if (p.getToken() == TOKEN_EXPONENT) {
            p.nextToken(); l.setExponent( p.getValue() );
        } else if (p.getToken() == TOKEN_AMBIENT) {
            p.nextToken(); ambientLight[0] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[1] = static_cast<float>(p.getValue());
            p.nextToken(); ambientLight[2] = static_cast<float>(p.getValue());
        } else {
            errorMessage = "Unrecognized token in light block: \"" + p.getToken() + "\"";
            return false;
        }

    } while (true);
    assert( l.getColor()[0] >= 0.0 && l.getColor()[0] <= 1.0 );
    assert( l.getColor()[1] >= 0.0 && l.getColor()[1] <= 1.0 );
    assert( l.getColor()[2] >= 0.0 && l.getColor()[2] <= 1.0 );
    assert( l.getAperture() >  0.0 && l.getAperture() <= 180.0 );
    assert( l.getExponent() >= 0.0 );
    lights.push_back(l);
    return true;
}

Tree* MyScene::parseMasterSubgraph(Parser& p) {
    // ToDo: parse a named subgraph tree
    p.nextToken();
    std::string name = p.getToken();
	if (masters->get(name) != 0) {
		errorMessage = "Found duplicate declaration of mastersubgraph: \"" + name + "\"";
		return false;
	}
	Tree* tree = new Tree();
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_TRANS) {
            // parse node and add it to the tree
            // call parseTrans(p);
			Node* n = parseTrans(p);
			if (n == 0) {  // if there was an error while parsing this node
				delete tree;
				return 0;  // report the error
			}
			tree->addNode(n);
        } else {
            errorMessage = "Unrecognized token in mastersubgraph \"" + name + "\": \"" + p.getToken() + "\"";
            delete tree;
			return 0;
        }
    } while (true);

    // add the new master subgraph to the master subgraph list
	masters->addTree(name, tree);
    return tree;}

Node* MyScene::parseTrans(Parser& p) {
    // parse a trans block node
	Node* n = new Node();
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_ROTATE) {
            Vector3 axis;
            p.nextToken(); axis[0] = p.getValue();
            p.nextToken(); axis[1] = p.getValue();
            p.nextToken(); axis[2] = p.getValue();
            p.nextToken(); double angle = p.getValue() * M_PI / 180.0;
            // ToDo: add a rotation to the matrix stack here
			Matrix4 r = Matrix4::rotation(axis, angle);
			Matrix4 rInv = Matrix4::rotation(axis, -angle);
			n->trans = n->trans * r;
			n->transInv = rInv * n->transInv;
			n->transInvT = n->transInvT * r;
        } else if (p.getToken() == TOKEN_TRANSLATE) {
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo: add a translation to the matrix stack here
			n->trans = n->trans * Matrix4::translation(v);
			n->transInv = Matrix4::translation(-v) * n->transInv;
        } else if (p.getToken() == TOKEN_SCALE) {
            Vector3 v;
            p.nextToken(); v[0] = p.getValue();
            p.nextToken(); v[1] = p.getValue();
            p.nextToken(); v[2] = p.getValue();
            // ToDo:add a scale to the matrix stack here
			Matrix4 s = Matrix4::scaling(v);
			Matrix4 sInv = Matrix4::scaling(Vector3(1.0/v[0], 1.0/v[1], 1.0/v[2]));

			n->trans = n->trans * s;
			n->transInv = sInv * n->transInv;
			n->transInvT = n->transInvT * sInv;
        } else if (p.getToken() == TOKEN_MATRIXRC) {
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r0[3] = p.getValue();
      
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
      
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
      
            p.nextToken(); r3[0] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
			Matrix4 m(r0, r1, r2, r3);
			n->trans = n->trans * m;
			n->transInv = m.inverse() * n->transInv;
			n->transInvT = m.inverse().transpose() * n->transInvT;
        } else if (p.getToken() == TOKEN_MATRIXCR) {
            Vector4 r0, r1, r2, r3;
            p.nextToken(); r0[0] = p.getValue();
            p.nextToken(); r1[0] = p.getValue();
            p.nextToken(); r2[0] = p.getValue();
            p.nextToken(); r3[0] = p.getValue();
      
            p.nextToken(); r0[1] = p.getValue();
            p.nextToken(); r1[1] = p.getValue();
            p.nextToken(); r2[1] = p.getValue();
            p.nextToken(); r3[1] = p.getValue();
      
            p.nextToken(); r0[2] = p.getValue();
            p.nextToken(); r1[2] = p.getValue();
            p.nextToken(); r2[2] = p.getValue();
            p.nextToken(); r3[2] = p.getValue();
      
            p.nextToken(); r0[3] = p.getValue();
            p.nextToken(); r1[3] = p.getValue();
            p.nextToken(); r2[3] = p.getValue();
            p.nextToken(); r3[3] = p.getValue();
            // ToDo: add the arbitrary matrix to the matrix stack here
			Matrix4 m(r0, r1, r2, r3);
			n->trans = n->trans * m;
			n->transInv = m.inverse() * n->transInv;
			n->transInvT = m.inverse().transpose() * n->transInvT;
        } else if (p.getToken() == TOKEN_OBJECT) {
            // ToDo: parse the object and add it to the node
            // call parseObject(p) here;
			if(n->object != 0) {
				errorMessage = "Found multiple objects in trans block";
				delete n;
				return 0;
			}
			n->object = parseObject(p);
			if (n->object == 0) {  // if there was an error while parsing this node
				delete n;
				return 0;		   // report the error
			}
        } else if (p.getToken() == TOKEN_SUBGRAPH) {
            // ToDo: find the subgraph and add it to the current node
            if (n->tree != 0) {
				errorMessage = "Found multiple trees in trans block";
				delete n;
				return 0;
			}
			p.nextToken();
            n->tree = masters->get(p.getToken());
			if (n->tree == 0) {  // if there was an error while parsing this node
				errorMessage = "reference to undeclared subgraph \"" + p.getToken() + "\"";
				delete n;
				return 0;
			}
        } else {
            errorMessage = "Unrecognized token in trans block: \"" + p.getToken() + "\"";
            delete n;
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return n;
}

Object* MyScene::parseObject(Parser& p) {
    // ToDo: parse an object block
	Object* o = new Object();

    p.nextToken();
    if (p.getToken() == TOKEN_CUBE) {
        // object is a cube
		o->shape = &cube;	
    } else if (p.getToken() == TOKEN_CYLINDER) {
        // object is a cylinder
		o->shape = &cylinder;	
    } else if (p.getToken() == TOKEN_CONE) {
        // object is a cone
		o->shape = &cone;	
    } else if (p.getToken() == TOKEN_SPHERE) {
        // object is a sphere
		o->shape = &sphere;	
    } else if (p.getToken() == TOKEN_COW) {
        // object is a cow (optional)
        // you may ignore this, or replace it with a different placeholder
        // shape if you wish

    } else {
        errorMessage = "Unrecognized object type: \"" + p.getToken() + "\"";
        delete o;
		return 0;
    }
    do {
        p.nextToken();
        if (p.getToken() == TOKEN_OB) {
        } else if (p.getToken() == TOKEN_CB) {
            break;
        } else if (p.getToken() == TOKEN_AMBIENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();

            // store ambient color here
			o->ambient = Color(r, g, b);
        } else if (p.getToken() == TOKEN_DIFFUSE) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store diffuse color here
			o->diffuse = Color(r, g, b);
        } else if (p.getToken() == TOKEN_SPECULAR) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store specular color here
			o->specular = Color(r, g, b);
        } else if (p.getToken() == TOKEN_REFLECT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store reflect color here
			o->reflect = Color(r, g, b);
        } else if (p.getToken() == TOKEN_TRANSPARENT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store transparent color here
			o->transparent = Color(r, g, b);
        } else if (p.getToken() == TOKEN_EMIT) {
            double r, g, b;
            p.nextToken(); r = p.getValue();
            p.nextToken(); g = p.getValue();
            p.nextToken(); b = p.getValue();
            // store emitted color here
			o->emit = Color(r, g, b);
        } else if (p.getToken() == TOKEN_SHINE) {
            double s;
            p.nextToken(); s = p.getValue();
            // store shine here
			o->shine = s;
        } else if (p.getToken() == TOKEN_IOR) {
            double ior;
            p.nextToken(); ior = p.getValue();
            // store ior here
			o->ior = ior;
        } else if (p.getToken() == TOKEN_TEXTURE) {
            std::string filename;
            double textureU, textureV;
            p.nextToken(); filename = p.getToken();
            p.nextToken(); textureU = p.getValue();
            p.nextToken(); textureV = p.getValue();
            // store texture information here
			o->textureU = textureU;
			o->textureV = textureV;
			o->filename = filename;
        } else {
            errorMessage = "Unrecognized token in object block: \"" + p.getToken() + "\"";
			delete o;
            return 0;
        }
    } while (true);

    // ToDo: Fix this
    return o;
}

std::string MyScene::getErrorMessage() const {
    // return the current error message
    return errorMessage;
}

const Camera & MyScene::getCamera() const {
    // return the current camera
    return camera;
}

Camera & MyScene::changeCamera() {
    // return the current camera
    return camera;
}

const Color &MyScene::getBackground() const {
    return background;
}
