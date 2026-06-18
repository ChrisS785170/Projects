import fs from "fs";
import path from "path";
import { pool } from "./connections.js";

const MIGRATIONS_DIR = "./migrations";

// Get all SQL files sorted
const files = fs
    .readdirSync(MIGRATIONS_DIR)
    .filter(file => file.endsWith(".sql"))
    .sort();

async function runMigrations() {
    console.log("Running migrations...");

    for (const file of files) {
        const filePath = path.join(MIGRATIONS_DIR, file);
        //take the contents from the file
        const sql = fs.readFileSync(filePath, "utf8");

        console.log(`Running: ${file}`);

        try {
            // execute the SQL we got from the file
            await pool.query(sql);
            console.log(`Success: ${file}`);
        } catch (err) {
            console.error(`Failed: ${file}`);
            console.error(err.message);
            process.exit(1);
        }
    }

    console.log("All migrations complete.");
    process.exit(0);
}

runMigrations();