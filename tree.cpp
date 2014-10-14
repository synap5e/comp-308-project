#include "tree.hpp"

#include "random.hpp"
#include "glm/gtx/string_cast.hpp"

Tree::Tree(){
	generate_crown();
	generate_trunk();
}


void Tree::generate_crown(){
	position = vec3(0,0,0);

    float PI = 3.141592653589793f;

    /*

    The unit teardrop is defined by 
    x = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi)
    y = 0.5 * (1-cosf(theta))
    z = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi)

    0 <= theta <= 2pi
    0 <= phi <= pi

    so

    choose a y from [0,1]
    theta = acos(1-2y)
    choose a phi [0,pi]
    
    */

    for (int i=0; i<attraction_point_count; ++i){
        float yu = RandomGen::get(0, 1);
        
        float theta = acosf(1-2*yu);
        float phi = RandomGen::get(0, PI);

        float rad = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi);
        rad = pow(rad, canopy_exponent);
       
        vec3 location = vec3(   RandomGen::get(-rad, rad) * radius,
                                yu * height,
                                RandomGen::get(-rad, rad) * radius);
        if (location.y < root_height){
            continue;
        }
        attraction_points.push_back(new AttractionPoint(location));

    }

}

void Tree::generate_trunk(){
	root = new Branch(nullptr, position + vec3(0, root_height, 0), vec3(0.f, 1.f, 0.f));
//    root->radius = 15;
	branches.push_back(root);
    live_branches.push_back(root);

}

int r = 0;
bool Tree::grow(){
    for (int i = 0; i < attraction_points.size(); ++i) {
    	AttractionPoint* attraction_point = attraction_points[i];

        attraction_point->closest = nullptr;
        vec3 direction;

        bool attraction_point_killed = false;

		for (Branch* b  : live_branches){
            direction = attraction_point->position - b->position;
            float distance = length(direction);
            direction = normalize(direction);


            if (distance < kill_distance * branch_length){
            	//printf("eat attraction_point\n");
                attraction_points.erase(attraction_points.begin()+i);                        
                i--;
                attraction_point_killed = true;
                break;
            } else if (distance < influence_distance * branch_length
            		   && ( attraction_point->closest == nullptr
            			 || length(attraction_point->position - attraction_point->closest->position) > distance)){
                    attraction_point->closest = b;
            }
        }
        

        if (!attraction_point_killed && attraction_point->closest) {
            vec3 dir = attraction_point->position - attraction_point->closest->position;
            dir = normalize(dir);
            attraction_point->closest->grow_direction += dir;
            attraction_point->closest->grow_count++;
        }
        
    }

    std::vector<Branch*> new_branches;
    for (int i=0;i<live_branches.size();i++){
        Branch* b = live_branches[i];
        if (b->grow_count > 0){
        	//std::cout << "Add branch, grow count:" << b->grow_count << "\n";

            vec3 avgDirection = b->grow_direction / float(b->grow_count+soft_bends_weight);
            avgDirection = normalize(avgDirection);

            b->grow_direction = b->original_grow_direction*float(soft_bends_weight);
            b->grow_count = 0;

            /*if (RandomGen::get(0, 2) > 1){
                continue;
            }*/


			vec3 new_pos = b->position + avgDirection * branch_length;

			if (std::find(branch_locations.begin(), branch_locations.end(), new_pos)!=branch_locations.end()){
				continue;
			}
			branch_locations.push_back(new_pos);

            Branch* newBranch = new Branch(b, new_pos, avgDirection);
            new_branches.push_back(newBranch);
            newBranch->radius = initial_radius;

            int pc = 0;
            Branch *parent = b;
            while (parent){
            	parent->radius += radius_growth;
            	parent = parent->parent;
            }
        } else {
            if (b->lifespan-- < 0){
                live_branches.erase(live_branches.begin()+i);
                ++i;
            }
        }

    }

    for (Branch* nb : new_branches){
        branches.push_back(nb);
        live_branches.push_back(nb);
    }


    if (new_branches.empty()){
        //return;
        for (AttractionPoint* l : attraction_points){
            delete l;
        }
        std::cout << "Finished with " << attraction_points.size() << " unused points\n";
    	attraction_points.clear();
        return false;
       /* for (Branch* b : branches){
            delete b;
        }
        branches.clear();
        generate_crown();
        generate_trunk();*/
    }

    return true;
}


std::vector<AttractionPoint*> Tree::get_attraction_points(){
    return attraction_points; 
};

std::vector<Branch*> Tree::get_branches(){
    return branches;
};
