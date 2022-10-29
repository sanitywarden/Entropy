GameSettings = {
    World = {
        -- Higher values might result in the application crashing.
        size = 64,
        terrain_from = 0.0,
        forest_from = 0.5,
        fog_of_war_enabled = false,
        multiplier_moisture = 0.35,   -- Increasing will result in more tropical world.
        multiplier_temperature = 0.75 -- Increasing will result in more arid world.
    },

    Region = {
        -- Higher values might result in the application crashing.
        -- Option not implemented, must not change size.
        size = 128,
        initial_population = 5,
        population_needs_enabled = false,
        building_cost_enabled = false,
        astar_pathfinding_enabled = false
    },
    
    world_persistence = 8,

    -- Persistence of the region noise generating function.
    -- The higher the value, the more persistent terrain.
    region_persistence = 64,
    
    -- Frequence of simulation updates.
    -- Update include the frequency of events, building updates, population updates etc.
    -- Lower value means more updates, but may heavily impact performance, or crash the application. 
    simulation_update_frequency = 1.0,
}