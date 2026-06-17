// we want to clean the raw food data before we insert it into the database
// this will be a function that takes in the raw food data and returns the cleaned food data
// we will use this function to clean the data before we insert it into the database

import tokenise from "./tokenise.js";

// This function handles the foodfacts data, preparing it for insertion into the database. 
// It takes in the raw food data and returns the cleaned food data.
export function clean_raw_food_facts(raw_foods){

    const cleanedFoods = [];

    for (raw_food of raw_foods) {
        const displayName = `${raw_food.brands} ${raw_food.raw_food_name}`

        const food = {
            name: raw_food.display_name,
            calories: raw_food.nutriments?.["energy-kcal_100g"],
            protein: raw_food.nutriments?.proteins_100g,
            carbs: raw_food.nutriments?.carbohydrates_100g,
            fat: raw_food.nutriments?.fat_100g,
            servingSize: raw_food.serving_size,
            nutriScore: raw_food.nutriscore_grade,
            tokens: tokenise(displayName)
        };
        cleanedFoods.push(food);
    }
    return cleanedFoods;
}