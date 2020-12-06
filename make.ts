console.log(
`  _ _ _
 | (_) |
 | |_| |__   ___ ___  _ __ ___
 | | | '_ \\ / __/ _ \\| '__/ _ \\
 | | | |_) | (_| (_) | | |  __/
 |_|_|_.__/ \\___\\___/|_|  \\___|
`);

import * as Path from 'https://deno.land/std/path/mod.ts';
import { Target, TargetType, Standard, red, green, yellow, remove, tempPath } from 'https://deno.land/x/buildsystem/mod.ts';

const mode: string = Deno.args.includes('--release') ? 'release' : 'debug';
const temp: string = Path.join(tempPath(), 'libcore', mode);
const project: string = Path.resolve(Deno.cwd());
const library: string = Path.resolve(Path.join(Deno.cwd(), '..'));
const platform: string = ({ windows: 'win32', darwin: 'macos', linux: 'linux' })[Deno.build.os];
const out_lib: string = Path.resolve(Path.join(`${project}`, 'out', 'lib', platform, mode, (platform == 'win32' ? 'core.lib' : 'libcore.a')));
const out_inc: string = Path.resolve(Path.join(`${project}`, 'out', 'include', 'libcore'));
const clean: boolean = (Deno.args.includes('--clean') || Deno.args.includes('--release'));

if (clean) {
    remove(temp);
    remove(out_lib);
    remove(out_inc);
}

await Promise.all([ Deno.mkdir(Path.dirname(out_lib), { recursive: true }),
                    Deno.mkdir(out_inc, { recursive: true }),
                    Deno.mkdir(temp, { recursive: true }) ]);

await Promise.all([ Deno.copyFile(`${project}/src/WINDOWS.h`, `${out_inc}/WINDOWS.h`),
                    Deno.copyFile(`${project}/src/MACRO.h`, `${out_inc}/MACRO.h`),
                    Deno.copyFile(`${project}/src/math.h`, `${out_inc}/math.h`),
                    Deno.copyFile(`${project}/src/line.h`, `${out_inc}/line.h`),
                    Deno.copyFile(`${project}/src/unicode.h`, `${out_inc}/unicode.h`),
                    Deno.copyFile(`${project}/src/keyboard.h`, `${out_inc}/keyboard.h`),
                    Deno.copyFile(`${project}/src/window.h`, `${out_inc}/window.h`),
                    Deno.copyFile(`${project}/src/color.h`, `${out_inc}/color.h`),
                    Deno.copyFile(`${project}/src/brush.h`, `${out_inc}/brush.h`),
                    Deno.copyFile(`${project}/src/bezierpath.h`, `${out_inc}/bezierpath.h`),
                    Deno.copyFile(`${project}/src/texture.h`, `${out_inc}/texture.h`),
                    Deno.copyFile(`${project}/src/context.h`, `${out_inc}/context.h`),
                    Deno.copyFile(`${project}/src/app.h`, `${out_inc}/app.h`) ]);

const target = new Target();
target.type = TargetType.library;
target.cStandard = Standard.c17;
target.includePath = [`${project}/out/include`];
target.arguments = ['-Wall', '-Wextra', '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-misleading-indentation'];
target.objcARC = true;
target.output = out_lib;
target.temp = temp;

target.sources.push(`${project}/src/bezierpath.c`);
target.sources.push(`${project}/src/unicode.c`);
target.sources.push(`${project}/src/brush.c`);
target.sources.push(`${project}/src/math.c`);

if (Deno.build.os == 'windows') {
    target.sources.push(`${project}/src/win32/app.c`);
    target.sources.push(`${project}/src/win32/window.c`);
} else if (Deno.build.os == 'darwin') {
    target.sources.push(`${project}/src/apple/app.m`);
    target.sources.push(`${project}/src/apple/window.m`);
    target.sources.push(`${project}/src/apple/context.m`);
    target.sources.push(`${project}/src/apple/texture.m`);
    target.sources.push(`${project}/src/apple/metal.m`);
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

const res = await target.make({ output: true, threads: 8, log: false });
if (res.type == 'error') {
    console.log(red('Unable to build libcore'));
    Deno.exit();
}

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

    if (clean) {
        remove(target.temp);
        remove(target.output);
    }

    if (Deno.args.includes('--release')) {
        target.debug = false;
        target.oLevel = 3;
        target.macros.push('NDEBUG');
    } else {
        target.debug = true;
        target.arguments.push('-fsanitize=address');
    }

    const res = await target.make({ output: true });
    if (res.type == 'error') {
        console.log(red('Unable to build example'));
        Deno.exit();
    }
    await Deno.run({ cmd: [ target.output ] }).status();
}
