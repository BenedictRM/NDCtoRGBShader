/*
 * ode.c
 *
 *  Created on: Jan 20, 2015
 *      Author: Russ Mehring
 */
#include "CSCIx239.h"
#include <ode/odeconfig.h>//Needed so that dInfinity macro is seen by these functions

//Assistance with ode from http://www.alsprogrammingresource.com/basic_ode.html
//and from the ODE user manual, not a lot of resources out there!
void initODE()
{
	dReal radius = 1.0;//For sphere's in scene that bounce around/ get shot
	// Create a new, empty world and assign its ID number to World. Most applications will only need one world.
    world = dWorldCreate();

    // Create a new collision space and assign its ID number to Space, passing 0 instead of an existing dSpaceID.
    // There are three different types of collision spaces we could create here depending on the number of objects
    // in the world but dSimpleSpaceCreate is fine for a small number of objects. If there were more objects we
    // would be using dHashSpaceCreate or dQuadTreeSpaceCreate (look these up in the ODE d
    spacePhy = dSimpleSpaceCreate(0);

    // Create a joint group object and assign its ID number to contactgroup. dJointGroupCreate used to have a
    // max_size parameter but it is no longer used so we just pass 0 as its argument.
    contactgroup = dJointGroupCreate(0);

    //Set the gravity of the world where y is up
    dWorldSetGravity(world, 0, -0.1, 0);

    // These next two functions control how much error correcting and constraint force mixing occurs in the world.
    // Don't worry about these for now as they are set to the default values and we could happily delete them from
    // this example. Different values, however, can drastically change the behaviour of the objects colliding, so
    // I suggest you look up the full info on them in the ODE docs.
    dWorldSetERP(world, 0.2);
    dWorldSetCFM(world, 1e-5);

    // This function sets the velocity that inter-penetrating objects will separate at. The default value is infinity.
    dWorldSetContactMaxCorrectingVel(world, 0.9);

    // This function sets the depth of the surface layer around the world objects. Contacts are allowed to sink into
    // each other up to this depth. Setting it to a small value reduces the amount of jittering between contacting
    // objects, the default value is 0.
    dWorldSetContactSurfaceLayer(world, 0.0);

    // To save some CPU time we set the auto disable flag to 1. This means that objects that have come to rest (based
    // on their current linear and angular velocity) will no longer participate in the simulation, unless acted upon
    // by a moving object. If you do not want to use this feature then set the flag to 0. You can also manually enable
    // or disable objects using dBodyEnable and dBodyDisable, see the docs for more info on this.
    dWorldSetAutoDisableFlag(world, 1);

    //This brings us to the end of the world settings, now we have to initialize the objects themselves.

    //****************Ball 1
    // Create a new body for our object in the world and get its ID.
    ball_body = dBodyCreate(world);
    //Set ball1 geometries
    dMassSetZero(&ball_mass);
    dMassSetSphereTotal(&ball_mass, DENSITY, 2.0);
    dBodySetMass(ball_body, &ball_mass);

    ball_geom = dCreateSphere(spacePhy, radius);
    dGeomSetData(ball_geom, (void *)"ball1");
    dGeomSetBody(ball_geom, ball_body);
    dBodyEnable(ball_body);
    //Next we set the position of the new body
	dBodySetPosition(ball_body, 0, 10, 0);
	//****************Ball 2
	// Create a new body for our object in the world and get its ID.
	ball_body2 = dBodyCreate(world);
	//Set ball1 geometries
	dMassSetZero(&ball_mass2);
	dMassSetSphereTotal(&ball_mass2, DENSITY, 2.0);
	dBodySetMass(ball_body2, &ball_mass2);

	ball_geom2 = dCreateSphere(spacePhy, radius);
	dGeomSetData(ball_geom2, (void *)"ball2");
	dGeomSetBody(ball_geom2, ball_body2);
	dBodyEnable(ball_body2);
	//Next we set the position of the new body
	dBodySetPosition(ball_body2, 2, 10, -1);
	//****************Ball 3
	// Create a new body for our object in the world and get its ID.
	ball_body3 = dBodyCreate(world);
	//Set ball1 geometries
	dMassSetZero(&ball_mass3);
	dMassSetSphereTotal(&ball_mass3, DENSITY, 2.0);
	dBodySetMass(ball_body3, &ball_mass3);

	ball_geom3 = dCreateSphere(spacePhy, radius);
	dGeomSetData(ball_geom3, (void *)"ball3");
	dGeomSetBody(ball_geom3, ball_body3);
	dBodyEnable(ball_body3);
	//Next we set the position of the new body
	dBodySetPosition(ball_body3, -2, 10, -1);
	//****************Ball 4
	// Create a new body for our object in the world and get its ID.
	ball_body4 = dBodyCreate(world);
	//Set ball1 geometries
	dMassSetZero(&ball_mass4);
	dMassSetSphereTotal(&ball_mass4, DENSITY, 2.0);
	dBodySetMass(ball_body4, &ball_mass4);

	ball_geom4 = dCreateSphere(spacePhy, (radius + 3));
	dGeomSetData(ball_geom4, (void *)"ball4");
	dGeomSetBody(ball_geom4, ball_body4);
	dBodyDisable(ball_body4);
	//Next we set the position of the new body
	dBodySetPosition(ball_body4, -0.5, 20, -0.25);


    //Set the ground location:
	//First three param's set the normal vector, last param sets the distance according to the plane equation equation a*x+b*y+c*z=d and must have length 1
	ground = dCreatePlane(spacePhy, 0.0, 0.01, 0, 0);//Plane where I have most objects sitting, y =0

    // Here I have set the initial linear velocity to stationary and let gravity do the work on our spheres, but you can experiment
    // with the velocity vector to change the starting behavior. You can also set the rotational velocity for the new
    // body using dBodySetAngularVel which takes the same parameters.
    tempVect.x = 0.0;
    tempVect.y = 0.0;
    tempVect.z = 0.0;
    dBodySetLinearVel(ball_body, tempVect.x, tempVect.y, tempVect.z);
    dBodySetLinearVel(ball_body2, tempVect.x, tempVect.y, tempVect.z);
    dBodySetLinearVel(ball_body3, tempVect.x, tempVect.y, tempVect.z);
    dBodySetLinearVel(ball_body4, tempVect.x, tempVect.y, tempVect.z);

    dWorldSetLinearDamping(world, 0.00001);
    dWorldSetAngularDamping(world, 0.005);
    dWorldSetMaxAngularSpeed(world, 200);
}


//Simulation Loop to test for collisions on every frame
void simLoop()
{
	//dSpaceCollide determines whih pairs of geoms are potentially interecting,
    //the callback function det's actual collisions before adding all the contact joints to groupcontact
    //This allows us control over the effect in the contact joints
    dSpaceCollide(spacePhy, 0, &nearCallback);

    // Now we advance the simulation by calling dWorldQuickStep. This is a faster version of dWorldStep but it is also
    // slightly less accurate. As well as the World object ID we also pass a step size value. In each step the simulation
    // is updated by a certain number of smaller steps or iterations. The default number of iterations is 20 but you can
    // change this by calling dWorldSetQuickStepNumIterations.
    dWorldQuickStep(world, 0.3);

    //Remove all temporary collision joints now that the world has been stepped
    dJointGroupEmpty(contactgroup);
}

//All pairs of geoms that are potentially intersecting will be passed by dSpaceCollide to this function.
//Here we can determine which objects are actually colliding by making a call to dCollide and change the
//behavior of the joints before adding them to the joint group. The first parameter would be the user data
//pointer passed to dSpaceCollide if we had provided it. The second and third parameters are the two potentially intersecting geoms.
void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    //Temporary index for each contact
    int i;
    int numc = 0;

    //Get the dynamics body for each geom
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    //Create an array of dContact objects to hold the contact joints
    dContact contact[MAX_CONTACTS];

    // Now we set the joint properties of each contact. Going into the full details here would require a tutorial of its
    // own. I'll just say that the members of the dContact structure control the joint behaviour, such as friction,
    // velocity and bounciness. See section 7.3.7 of the ODE manual and have fun experimenting to learn more.

    for (i = 0; i < MAX_CONTACTS; i++)
    {
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = dInfinity;
        contact[i].surface.mu2 = 5000;
        contact[i].surface.bounce = 0.98;
        contact[i].surface.bounce_vel = 0.5;
        contact[i].surface.soft_cfm = 0.01;
    }

    // Here we do the actual collision test by calling dCollide. It returns the number of actual contact points or zero
    // if there were none. As well as the geom IDs, max number of contacts we also pass the address of a dContactGeom
    // as the fourth parameter. dContactGeom is a substructure of a dContact object so we simply pass the address of
    // the first dContactGeom from our array of dContact objects and then pass the offset to the next dContactGeom
    // as the fifth paramater, which is the size of a dContact structure. That made sense didn't it?
    numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
    if (numc > 0)
    {
        // To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
        // different joint types available.
        for (i = 0; i < numc; i++)
        {
            //dJointCreateContact needs to know which world and joint group to work with as well as the dContact
            //object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
            //temporary contact joint between the two geom bodies.

            dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
            dJointAttach(c, b1, b2);
        }
    }
}

