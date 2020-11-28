/*! \mainpage CSCI 3081W Drone Simulation Project
 *
 * # Introduction
 *
 * This page contains supplementary documentation for the CSCI 3081W
 * Drone Simulation project, which handles the creation, scheduling, and drone delivery
 * of ACME packages to our customers.
 *
 * ## Description/Methodology
 * This implementation takes advantage of a Facade pattern in the form of the DroneSimulation
 * class, which inherits solely from the given DroneDeliverySystem and from which all other
 * classes are implemented from (except for the entities themselves which inherit from the
 * given entity_project::Entity class). A concrete factory is used to create entities when
 * the `CreateEntity()` method is called in the DroneSimulation class, and upon an `AddEntity()`
 * call, are stored in a vector within the DroneSimulation class. The Package Observer notifies 
 * clients on the status of a given package. The Observer receives the notifications of scheduled, 
 * en route and delivered depending on which state the package is in. 
 *
 * The simulation itself is handled independently of calls into the system (thanks to the Facade),
 * with the Scheduler determining which drone should deliver which package to its customer,
 * and the DroneSimulation class calling its `Update()` function over and over to increment
 * the simulation accordingly.
 *
 * Each entity type (Drone, Customer, or Package) can communicate with each other using "statuses"
 * which are unique to a specific type. Upon a change to an entity's status, that entity may
 * execute code that changes its behavior in the simulation.  For example, a package that is 
 * "Delivered"  or a customer that has "Received [a] package" will immediately remove itself
 * from the system, or a drone that is "Waiting" will automatically search for and assign itself
 * a package that is "Ready to ship". This status can be extended by either adding new statuses or
 * replacing existing code with more sophisticated algorithms. Right now, statuses are baked into 
 * the entity classes, but they might be able to be separated into separate classes to more
 * accurately reflect the "Open to extension, closed to modification" design principle.
 *
 * Included in the code is a csci3081::Vector3 class that implements a basic 3-dimensional vector
 * that implements basic vector addition, scalar multiplication, normalization, and length
 * calculation. A Vector3 can also be converted into a SimpleUMNRoutePoint for implementation
 * in route-planning algorithms in the future.
 *
 * ## UML Diagram
 * \image html Iteration_2_UML.png width=100%
 */
