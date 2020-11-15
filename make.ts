console.log(
`  _ _ _                        
 | (_) |                       
 | |_| |__   ___ ___  _ __ ___ 
 | | | '_ \\ / __/ _ \\| '__/ _ \\
 | | | |_) | (_| (_) | | |  __/
 |_|_|_.__/ \\___\\___/|_|  \\___|
`);

import * as Path from 'https://deno.land/std/path/mod.ts';
import { Target, TargetType, Standard, tempPath } from '../buildsystem/mod.ts';

const mode: string = Deno.args.includes('--release') ? 'release' : 'debug';
const temp: string = Path.join(tempPath(), 'libcore', mode);

if (Deno.args.includes('--clean')
|| Deno.args.includes('--release')) {
    try {
        Deno.removeSync(temp, { recursive: true });
    } catch (e) {}
}

const project: string = Path.resolve(Deno.cwd());
const library: string = Path.resolve(Path.join(Deno.cwd(), '..'));
const platform: string = ({ windows: 'win32', darwin: 'macos', linux: 'linux' })[Deno.build.os];
const out_lib: string = Path.resolve(Path.join(`${project}`, 'out', 'lib', platform, mode, (platform == 'win32' ? 'core.lib' : 'libcore.a')));
const out_inc: string = Path.resolve(Path.join(`${project}`, 'out', 'include', 'libcore'));

await Promise.all([ Deno.mkdir(Path.dirname(out_lib), { recursive: true }),
                    Deno.mkdir(out_inc, { recursive: true }),
                    Deno.mkdir(temp, { recursive: true }) ]);

await Promise.all([ Deno.copyFile(`${project}/src/WINDOWS.h`, `${out_inc}/WINDOWS.h`),
                    Deno.copyFile(`${project}/src/MACRO.h`, `${out_inc}/MACRO.h`),
                    Deno.copyFile(`${project}/src/size.h`, `${out_inc}/size.h`),
                    Deno.copyFile(`${project}/src/point.h`, `${out_inc}/point.h`),
                    Deno.copyFile(`${project}/src/rect.h`, `${out_inc}/rect.h`),
                    Deno.copyFile(`${project}/src/keyboard.h`, `${out_inc}/keyboard.h`),
                    Deno.copyFile(`${project}/src/window.h`, `${out_inc}/window.h`),
                    Deno.copyFile(`${project}/src/app.h`, `${out_inc}/app.h`) ]);

const target = new Target();
target.type = TargetType.library;
target.cStandard = Standard.c17;
target.includePath = [`${project}/out/include`];
target.arguments = ['-Wall', '-Wextra', '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-misleading-indentation'];
target.objcARC = true;
target.output = out_lib;
target.temp = temp;

if (Deno.build.os == 'windows') {
    target.sources.push(`${project}/src/win32/app.c`);
} else if (Deno.build.os == 'darwin') {
    target.sources.push(`${project}/src/apple/app.m`);
    target.sources.push(`${project}/src/apple/window.m`);
} else {
    throw new Error(`${Deno.build.os} not supported for build target`);
}

if (Deno.args.includes('--release')) {
    target.debug = false;
    target.oLevel = 3;
    target.macros.push('NDEBUG');
} else {
    target.debug = true;
    target.arguments.push('-fsanitize=address');
}

await target.make({ output: true, threads: 8, log: false });

if (Deno.args.includes('--example')) {
    const target = new Target();
    target.type = TargetType.executable;
    target.temp = Path.join(temp, 'example');
    target.output = Path.join(target.temp, `example${(Deno.build.os == 'windows') ? '.exe' : '.a'}`);
    target.cStandard = Standard.c17;
    target.includePath = [Path.dirname(out_inc)];
    target.libraryPath = [Path.dirname(out_lib)];
    target.libraries = ['core'];
    target.sources.push(`${project}/example/main.c`);
    if (Deno.build.os == 'windows') {

    } else if (Deno.build.os == 'darwin') {
        target.frameworks = ['Cocoa', 'Metal', 'MetalKit', 'QuartzCore'];
    }

    if (Deno.args.includes('--release')) {
        target.debug = false;
        target.oLevel = 3;
        target.macros.push('NDEBUG');
    } else {
        target.debug = true;
        target.arguments.push('-fsanitize=address');
    }

    await target.make({ output: true });
    await Deno.run({ cmd: [ target.output ] }).status();
}
