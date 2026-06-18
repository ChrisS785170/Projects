import fs from 'fs';
import { clean_raw_USDA } from '../Helpers/clean_raw_USDA.js';
import { bulk_insert_foods } from '../repositories/food_repository.js';
import { insert_food_tokens } from '../repositories/food_tokens_repository.js';



const data = JSON.parse(
  fs.readFileSync("../seeds/FoodData_Central_foundation_food_json_2025-04-24.json", "utf8")
);

const foods = data.FoundationFoods;

// ProcessFoodBatch(foods); // this will be piped into a cleaning function, then piped into a db function
const cleaned_foods = clean_raw_USDA(foods);

const [result] = await bulk_insert_foods(cleaned_foods);
await insert_food_tokens(result.insertId, cleaned_foods);
console.log("Finished inserting USDA foods into database");