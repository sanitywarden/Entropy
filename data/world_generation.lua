GameSettings = {
    World = {
        size = 128,
        terrain_from = 0.2,
        forest_from = 0.6,
        fog_of_war_enabled = false
    },

    Region = {
        size = 128,
        initial_population = 5,
        population_needs_enabled = false,
        building_cost_enabled = false,
        astar_pathfinding_enabled = false
    },
    
    -- Persistence of the noise generating function.
    -- The higher the value, the more persistent terrain.
    persistence = 64,
    
    -- Frequence of simulation updates.
    -- Update include the frequency of events, building updates, population updates etc.
    -- Lower value means more updates, but may heavily impact performance, or crash the application. 
    simulation_update_frequency = 1.0
}