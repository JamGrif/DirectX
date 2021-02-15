#pragma once
#include "EngineStatics.h"

#include <d3d11.h>

#include <stdio.h>
#include <string>
#include <vector>



struct MODEL_POS_TEX_NORM_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Normal;
};

class MeshLoader
{
public:
	MeshLoader();
	~MeshLoader();

	void LoadObj(char* fname, ID3D11Buffer*& vb, unsigned int& numverts);


private:

	//Mesh loading
	int loadfile(char* fname);
	void parsefile(char* filename);
	bool getnextline();
	bool getnexttoken(int& tokenstart, int& tokenlength);
	bool createVB(ID3D11Buffer*& vb, unsigned int& numverts);


private:

	MODEL_POS_TEX_NORM_VERTEX* vertices;

	char* fbuffer;
	long fbuffersize; // filesize
	size_t actualsize; // actual size of loaded data (can be less if loading as text files as ASCII CR (0d) are stripped out)
	unsigned int  tokenptr;
	struct xyz { float x, y, z; };	//used for vertices and normals during file parse
	struct xy { float x, y; };		//used for texture coordinates during file parse
	std::string filenameString;

	std::vector <xyz> position_list;		// list of parsed positions
	std::vector <xyz> normal_list;		// list of parsed normals
	std::vector <xy> texcoord_list;		// list of parsed texture coordinates
	std::vector <int> pindices, tindices, nindices; // lists of indicies into above lists derived from faces

};

