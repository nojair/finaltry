# Configuración de las funciones de crossover, mutación, seleccción y evaluación de individuos
from deap import base, creator

# Configuración para buscar controladores que minimizen el error
creator.create("ControllerFitness", base.Fitness, weights=(-1.0,))
# Creación de la clase para crear individuos (cromosomas)
creator.create("Controller", list, fitness=creator.ControllerFitness)

import random
from deap import tools

# ki bits + kp bits + kd bits
IND_SIZE = 30

# Configuración de los individuos como palabras binarias
toolbox = base.Toolbox()
toolbox.register("bit_attribute", random.randint, 0, 1)
toolbox.register("controller", tools.initRepeat, creator.Controller, toolbox.bit_attribute, n=IND_SIZE)
toolbox.register("population", tools.initRepeat, list, toolbox.controller)

toolbox.register("mate", tools.cxTwoPoint)
toolbox.register("mutate", tools.mutUniformInt, low=0, up=1, indpb=0.1)
toolbox.register("select", tools.selTournament, tournsize=3)
toolbox.register("evaluate", evaluate) # "evaluate": Misma función que decodifica y evalua el desempeño de un controlador

def main():
    # Inicializar población
    pop = toolbox.population(n=50)
    # Factores de crossover, mutación y número de generaciones
    CXPB, MUTPB, NGEN = 0.7, 0.5, 50
    # Evaluar la población
    fitnesses = list(map(toolbox.evaluate, pop))

    for ind, fit in zip(pop, fitnesses):
        ind.fitness.values = fit
        minFit = (1,)

    for g in range(NGEN):
        # Seleccionando la siguiente generación
        offspring = toolbox.select(pop, len(pop))
        # Clonando los individuos seleccionados
        offspring = list(map(toolbox.clone, offspring))
        # Aplicando crossover y mutación
        for child1, child2 in zip(offspring[::2],
            ffspring[1::2]):
            if random.random() < CXPB:
                toolbox.mate(child1, child2)
                del child1.fitness.values
                del child2.fitness.values

        for mutant in offspring:
            if random.random() < MUTPB:
                toolbox.mutate(mutant)
                del mutant.fitness.values
                # Evaluación de desempeño de los individuos con un finess inválido mediante

        invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
        fitnesses = list(map(toolbox.evaluate, invalid_ind))
        minFit = min(fitnesses)
        
        for ind, fit in zip(invalid_ind, fitnesses):
            ind.fitness.values = fit
        # The population is entirely replaced by the offspring
        pop[:] = offspring

    return pop

best_controllers_pop = main()