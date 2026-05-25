import fs from 'fs';



const data = JSON.parse(
  fs.readFileSync("../seeds/FoodData_Central_foundation_food_json_2025-04-24.json", "utf8")
);

const foods = data.FoundationFoods;

// ProcessFoodBatch(foods); // this will be piped into a cleaning function, then piped into a db function
console.log(foods.length);
