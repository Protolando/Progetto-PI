#include "gameMap.hpp"
#include "../json.hpp"
#define TILESIZE 75

using namespace nlohmann;

GameMap::GameMap(std::string path){
	GameMap::texturemap = NULL;
	GameMap::bumpmap = NULL;
	GameMap::importMap(path);
}

GameMap::~GameMap(){
	for(int i = 0; i < GameMap::mapHeight; i++){
			free(GameMap::texturemap[i]);
			free(GameMap::bumpmap[i]);
	}
	free(GameMap::texturemap);
	free(GameMap::bumpmap);
	
	for(auto i = GameMap::textures.begin(); i != GameMap::textures.end(); i++){
		free(*i);
	}
}

void GameMap::draw(glm::mat4 view){
	for(int i = 0; i < GameMap::mapHeight; i++){
		for(int j = 0; j < GameMap::mapWidth; j++){
			if(GameMap::texturemap[i][j] == -1){
				continue;
			}
			GLint pics_program = Context::getTexureProgram();
			glUseProgram(pics_program);
			
			texture_t *currtexture = GameMap::textures.at(GameMap::texturemap[i][j]);
	 		glBindTexture(
	 			GL_TEXTURE_2D,
	 			currtexture->buffer
	 		);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glm::vec2 tilepos = {TILESIZE*j, TILESIZE*i};

			//Put the vertices position in an array
			
			float points[] = {
				tilepos.x, tilepos.y,
				tilepos.x, tilepos.y + TILESIZE,
				tilepos.x + TILESIZE, tilepos.y + TILESIZE,
				tilepos.x + TILESIZE, tilepos.y,
			};
	
			float tex_coord[] = {
				0.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f
			};

			// Create an element array
			GLuint ebo;
			glGenBuffers(1, &ebo);

			GLuint elements[] = {
				0, 1, 2,
				2, 3, 0
			};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
			
			//Load the vertices position into the GPU buffers
			GLuint points_vbo = 0;
			glGenBuffers(1, &points_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

			GLint attrib = Context::renderer->get_attrib(pics_program, "position");
			glEnableVertexAttribArray(attrib);
			glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

			GLuint tex_coord_vbo = 0;
			glGenBuffers(1, &tex_coord_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
			GLint textcoord = Context::renderer->get_attrib(pics_program, "texcoord");
			glEnableVertexAttribArray(textcoord);
			glVertexAttribPointer(textcoord, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

			GLint uloc = Context::renderer->get_uniform(pics_program, "projection");
			glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(Context::projection * view));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glDeleteBuffers(1, &points_vbo);
			glDeleteBuffers(1, &tex_coord_vbo);
			glDeleteBuffers(1, &ebo);
		
			/*if(GameMap::showTileNumber){
				char *tmp = (char*) malloc(sizeof(char)*10);
				sprintf(tmp, "(%d, %d)", j, i);
				glm::vec4 postmp = view * glm::vec4(tilepos, 1.0f, 1.0f);
				
				Context::renderer->draw_text(
					std::string(tmp),
					{postmp.x, postmp.y},
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
					10
				);
				free(tmp);
			}*/
		}
    }
}

void GameMap::importMap(std::string path){
	json map = json::parse(Context::readFromFile(path.c_str()));

	GameMap::mapHeight = map["nrows"].get<int>();
	GameMap::mapWidth = map["ncols"].get<int>();
	
	/*Allocating data structures*/
	GameMap::texturemap = (int **) malloc(sizeof(int *) * GameMap::mapHeight);
	for(int i = 0; i < GameMap::mapHeight; i++){
		GameMap::texturemap[i] = (int *) malloc(sizeof(int) * GameMap::mapWidth);
	}
		
	GameMap::bumpmap = (BumpmapValues **) malloc(sizeof(BumpmapValues *) * GameMap::mapHeight);
	for(int i = 0; i < GameMap::mapHeight; i++){
		GameMap::bumpmap[i] = (BumpmapValues *) malloc(sizeof(BumpmapValues) * GameMap::mapWidth);
	}
	
	/*Importing textures*/
	for(int i = 0; i < map["ntextures"].get<int>(); i++){
		GameMap::textures.push_back(
			Context::renderer->generate_texture(map["textures"][i].get<std::string>().c_str())
		);
	}
	
	/*Importing data*/
	for(int i = 0; i < GameMap::mapHeight; i++){
		for(int j = 0; j < GameMap::mapWidth; j++){
			GameMap::texturemap[i][j] = map["map"][i][j].get<int>();
			GameMap::bumpmap[i][j] = valuesArray[map["bumpmap"][i][j].get<int>()];
		}
	}
}

bool GameMap::isWalkable(Rect r){
	bool ret = true;
	
	int nTilesX = ceil((r.getBotRight().x)/TILESIZE) - floor(r.getTopLeft().x/TILESIZE);
	int nTilesY = ceil((r.getBotRight().y)/TILESIZE) - floor(r.getTopLeft().y/TILESIZE);
	
	int x = floor((r.getTopLeft().x)/TILESIZE);
	int y = floor((r.getTopLeft().y)/TILESIZE);
	
	for(int i = 0; i < nTilesX; i++){
		for(int j = 0; j < nTilesY; j++){
			if(x+i < 0 || y+j < 0 || x+i > GameMap::mapWidth || y+j > GameMap::mapHeight)
				return false;
			else{
				ret = ret && bumpmap[y+j][x+i] != BumpmapValues::WALL;
			}
		}
	}

	return ret;
}

void GameMap::toggleTileNumber(){
	GameMap::showTileNumber = !GameMap::showTileNumber;
}

int GameMap::getWidth(){
	return GameMap::mapWidth;
}

int GameMap::getHeight(){
	return GameMap::mapHeight;
}
