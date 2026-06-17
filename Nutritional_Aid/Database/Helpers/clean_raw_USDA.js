import tokenise from "./tokenise.js";
// This function takes in an array of food objects from the USDA API and returns an array of cleaned food objects with only the relevant information for our database.
function cleanraw_USDA(foods) {
    const cleanedFoods = [];
    for (food of foods) {

        const displayName = `${food.brands} ${food.description}`
        const cleanedFood = {
            name: displayName,
            calories: food.foodNutrients?.find(nutrient => nutrient.nutrientName === "Energy")?.value,
            protein: food.foodNutrients?.find(nutrient => nutrient.nutrientName === "Protein")?.value,
            carbs: food.foodNutrients?.find(nutrient => nutrient.nutrientName === "Carbohydrate, by difference")?.value,
            fat: food.foodNutrients?.find(nutrient => nutrient.nutrientName === "Total lipid (fat)")?.value,
            servingSize: food.servingSize,
            tokens: tokenise(displayName)
        };
        cleanedFoods.push(cleanedFood);
    }
    return cleanedFoods;
}