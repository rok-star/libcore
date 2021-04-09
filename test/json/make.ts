import { extname, basename, dirname, resolve, fromFileUrl } from 'https://deno.land/std/path/mod.ts'
import { walkSync } from 'https://deno.land/std/fs/mod.ts'

const { readTextFileSync, writeTextFileSync } = Deno;

const __dirname: string = dirname(fromFileUrl(import.meta.url));

const make = (path: string): void => {

	let seq = 1;

	const escape = (str: string) => str.replace(/[\\]/g, '\\\\').replace(/[\"]/g, '\\\"').replace(/[\b]/g, '\\b')
										.replace(/[\f]/g, '\\f').replace(/[\n]/g, '\\n').replace(/[\r]/g, '\\r')
										.replace(/[\t]/g, '\\t');
	const genName = () => `tmp${seq++}`;

	const name = basename(path).replace(extname(path), '');
	const json = readTextFileSync(path);
	const value = JSON.parse(json);
	const output: string[] = [];

	const makeObject = (value: Record<string, any>, name: string): void => {
		output.push(`	_ASSERT(_value_type(${name}) == _MAP_VALUE_TYPE);`);
		for (const key of Object.keys(value)) {
			output.push(`	_ASSERT(_value_map_has(${name}, "${escape(key)}", ${key.length}) == true)`);
		}
		for (const key of Object.keys(value)) {
			const name2 = genName();
			output.push(``);
			output.push(`	_value_t* ${name2} = _value_map_get(${name}, "${escape(key)}", ${key.length});`);
			makeValue(value[key], name2);
		}
	}

	const makeBoolean = (value: boolean, name: string): void => {
		output.push(`	_ASSERT(_value_type(${name}) == _BOOL_VALUE_TYPE);`);
		output.push(`	_ASSERT(_value_bool(${name}) == ${value.toString()});`);
	}

	const makeNumber = (value: number, name: string): void => {
		output.push(`	_ASSERT(_value_type(${name}) == _NUMBER_VALUE_TYPE);`);
		output.push(`	_ASSERT(_value_number(${name}) == ${value.toString()});`);
	}

	const makeString = (value: string, name: string): void => {
		output.push(`	_ASSERT(_value_type(${name}) == _STRING_VALUE_TYPE);`);
		output.push(`	_ASSERT(_string_compare(_value_string(${name}), strlen(_value_string(${name})), "${escape(value)}", ${value.length}) == true);`);
	}

	const makeArray = (array: any[], name: string): void => {
		output.push(`	_ASSERT(_value_type(${name}) == _ARRAY_VALUE_TYPE);`);
		output.push(`	_ASSERT(_value_array_count(${name}) == ${array.length});`);
		for (let i = 0; i < array.length; i++) {
			const name2 = genName();
			output.push(``);
			output.push(`	_value_t* ${name2} = _value_array_get(${name}, ${i});`);
			makeValue(array[i], name2);
		}
	}

	const makeValue = (value: any, name: string): void => {
		if (Array.isArray(value)) {
			makeArray(value, name);
		} else if (typeof value === 'object') {
			makeObject(value, name);
		} else if (typeof value === 'number') {
			makeNumber(value, name);
		} else if (typeof value === 'string') {
			makeString(value, name);
		} else if (typeof value === 'boolean') {
			makeBoolean(value, name);
		} else {
			throw new Error(`what about "${typeof value}"?`)
		}
	}

	output.push(`void test_${name}(void) {`);
	output.push(`	char const json[] = { ${(new TextEncoder()).encode(json).join(', ')}, 0 };`);
	output.push(`	_status_t status = {};`);
	output.push(``);
	output.push(`	_value_t* root = _json_parse(json, sizeof json, &status);`);

	makeValue(value, 'root');

	output.push(`}`);

	writeTextFileSync(resolve(__dirname, 'output', `${name}.c`), output.join('\n'));
}

make(resolve(__dirname, 'input/sample1.json'));


const files: string[] = [];

for (const entry of walkSync(resolve(__dirname, 'input'), { exts: ['.json'], maxDepth: 1 })) {
	make(entry.path);
	files.push(`output/${entry.name.replace(extname(entry.name), '')}.c`);
}

writeTextFileSync(resolve(__dirname, 'index.c'),
`#include <libcore/status.h>
#include <libcore/string.h>
#include <libcore/value.h>
#include <libcore/json.h>
#include <libcore/MACRO.h>

${files.map(f => `#include "${f}"`)}`);