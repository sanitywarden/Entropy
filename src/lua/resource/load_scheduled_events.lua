time_per_hour  = 12
time_per_day   = 12 * time_per_hour
time_per_week  = 7  * time_per_day
time_per_month = 4  * time_per_week
time_per_year  = 12 * time_per_month

loadScheduledEvent("onUpdateUnit"    , 1            , false)
loadScheduledEvent("onProductionTick", time_per_hour, false)
loadScheduledEvent("onHarvestTick"   , time_per_hour, false)
loadScheduledEvent("onPopulationTick", time_per_day , false)