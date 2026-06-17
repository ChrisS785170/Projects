import zlib from "zlib";
import fs from "fs";
import readline from "readline";
import clean_raw_food_facts from "../Helpers/clean_raw_food_facts.js";
import { bulk_insert_foods } from "../repositories/food_repository.js";


const PATH = '../seeds/openfoodfacts-products.jsonl.gz'

// Create a read stream for the gzipped file and pipe it through zlib to decompress
const stream = fs.createReadStream(PATH).pipe(zlib.createGunzip());

// Create a readline interface to read the decompressed data line by line
const rl = readline.createInterface({
  input: stream,
  crlfDelay: Infinity
});

// Listen for the 'line' event to process each line of the file
// this line event happens whenever a new line is read from the file, it will trigger the callback function that takes the line as an argument

const batchSize = 1000; // Adjust the batch size as needed
let batch = [];

rl.on('line', (line) => {
  try {
    const Foodobject = JSON.parse(line);
    // Process the Foodobject as needed
    batch.push(Foodobject);

    if (batch.length >= batchSize) {
        rl.pause(); // Pause the readline interface to process the batch
      // Process the batch
        const cleanedBatch = clean_raw_food_facts(batch);
        await bulk_insert_foods(cleanedBatch);
        batch = [];
        rl.resume(); // Resume the readline interface to continue reading lines
    }
  } catch (err) {
    console.error('Error parsing JSON:', err);
  }
});

    if (batch.length > 0) {
        // Process any remaining items in the batch
        const cleanedBatch = clean_raw_food_facts(batch);
        await bulk_insert_foods(cleanedBatch);
    }

rl.on('close', () => {
  console.log('Finished processing the Food Facts Database .');
});
