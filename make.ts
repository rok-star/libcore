console.log(`
  _ _ _
 | (_) |
 | |_| |__   ___ ___  _ __ ___
 | | | '_ \\ / __/ _ \\| '__/ _ \\
 | | | |_) | (_| (_) | | |  __/
 |_|_|_.__/ \\___\\___/|_|  \\___|
`);

import * as Path from 'https://deno.land/std/path/mod.ts';
import { Target, TargetType, Standard, red, green, yellow, remove, tempPath } from 'https://deno.land/x/buildsystem/mod.ts';
//import { Target, TargetType, Standard, red, green, yellow, remove, tempPath } from '../buildsystem/mod.ts';

const mode: string = Deno.args.includes('--release') ? 'release' : 'debug';
const temp: string = Path.join(tempPath(), 'libcore', mode);
const dirname: string = Path.dirname(Path.fromFileUrl(import.meta.url));
const project: string = Path.resolve(dirname);
const library: string = Path.resolve(Path.join(dirname, '..'));
const out_lib: string = Path.resolve(Path.join(`${project}`, 'out', 'lib', Deno.build.os, mode, (Deno.build.os == 'windows' ? 'ext.lib' : 'libcore.a')));
const out_inc: string = Path.resolve(Path.join(`${project}`, 'out', 'include', 'libcore'));
const clean: boolean = (Deno.args.includes('--clean') || Deno.args.includes('--release'));
const cpus: number = await (async (): Promise<number> => {
    if (Deno.build.os == 'linux') {
        const proc = Deno.run({ cmd: ["nproc"], stdout: "piped", stderr: "piped" });
        const out = await proc.output();
        const str = new TextDecoder().decode(out);
        const ret: number = +(str);
        proc.close();
        return ret;
    } else {
        return 8;
    }
})();

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
                    Deno.copyFile(`${project}/src/status.h`, `${out_inc}/status.h`),
                    Deno.copyFile(`${project}/src/math.h`, `${out_inc}/math.h`),
                    Deno.copyFile(`${project}/src/unicode.h`, `${out_inc}/unicode.h`),
                    Deno.copyFile(`${project}/src/string.h`, `${out_inc}/string.h`),
                    Deno.copyFile(`${project}/src/parser.h`, `${out_inc}/parser.h`),
                    Deno.copyFile(`${project}/src/keyboard.h`, `${out_inc}/keyboard.h`),
                    Deno.copyFile(`${project}/src/color.h`, `${out_inc}/color.h`),
                    Deno.copyFile(`${project}/src/brush.h`, `${out_inc}/brush.h`),
                    Deno.copyFile(`${project}/src/bezierpath.h`, `${out_inc}/bezierpath.h`),
                    Deno.copyFile(`${project}/src/texture.h`, `${out_inc}/texture.h`),
                    Deno.copyFile(`${project}/src/context.h`, `${out_inc}/context.h`),
                    Deno.copyFile(`${project}/src/layer.h`, `${out_inc}/layer.h`),
                    Deno.copyFile(`${project}/src/signal.h`, `${out_inc}/signal.h`),
                    Deno.copyFile(`${project}/src/net.h`, `${out_inc}/net.h`),
                    //Deno.copyFile(`${project}/src/http.h`, `${out_inc}/http.h`),
                    Deno.copyFile(`${project}/src/thread.h`, `${out_inc}/thread.h`),
                    Deno.copyFile(`${project}/src/timer.h`, `${out_inc}/timer.h`),
                    Deno.copyFile(`${project}/src/time.h`, `${out_inc}/time.h`),
                    Deno.copyFile(`${project}/src/cond.h`, `${out_inc}/cond.h`),
                    Deno.copyFile(`${project}/src/lock.h`, `${out_inc}/lock.h`),
                    Deno.copyFile(`${project}/src/sleep.h`, `${out_inc}/sleep.h`),
                    Deno.copyFile(`${project}/src/window.h`, `${out_inc}/window.h`),
                    Deno.copyFile(`${project}/src/app.h`, `${out_inc}/app.h`),
                    Deno.copyFile(`${project}/src/path.h`, `${out_inc}/path.h`),
                    Deno.copyFile(`${project}/src/file.h`, `${out_inc}/file.h`),
                    Deno.copyFile(`${project}/src/json.h`, `${out_inc}/json.h`),
                    Deno.copyFile(`${project}/src/value.h`, `${out_inc}/value.h`),
                    Deno.copyFile(`${project}/src/directory.h`, `${out_inc}/directory.h`),
                    Deno.copyFile(`${project}/src/dispatchqueue.h`, `${out_inc}/dispatchqueue.h`) ]);

const target = new Target();
target.type = TargetType.library;
target.cStandard = Standard.c17;
target.includePath = [`${project}/out/include`];
target.arguments = ['-Wall', '-Wextra', '-Wno-unused-parameter', '-Wno-unused-variable', '-Wno-misleading-indentation'];
target.objcARC = true;
target.output = out_lib;
target.temp = temp;

target.sources.push(`${project}/src/dispatchqueue.c`);
target.sources.push(`${project}/src/bezierpath.c`);
target.sources.push(`${project}/src/unicode.c`);
target.sources.push(`${project}/src/string.c`);
target.sources.push(`${project}/src/parser.c`);
target.sources.push(`${project}/src/status.c`);
target.sources.push(`${project}/src/brush.c`);
target.sources.push(`${project}/src/layer.c`);
target.sources.push(`${project}/src/color.c`);
target.sources.push(`${project}/src/math.c`);
target.sources.push(`${project}/src/path.c`);
target.sources.push(`${project}/src/json.c`);
target.sources.push(`${project}/src/value.c`);

if (Deno.build.os == 'windows') {
    target.sources.push(`${project}/src/win32/app.c`);
    target.sources.push(`${project}/src/win32/window.c`);
} else if (Deno.build.os == 'darwin') {
    target.sources.push(`${project}/src/apple/app.m`);
    target.sources.push(`${project}/src/apple/window.m`);
    target.sources.push(`${project}/src/apple/context.m`);
    target.sources.push(`${project}/src/apple/texture.m`);
    target.sources.push(`${project}/src/apple/metal.m`);
    target.sources.push(`${project}/src/apple/timer.m`);
    target.sources.push(`${project}/src/POSIX/cond.c`);
    target.sources.push(`${project}/src/POSIX/lock.c`);
    target.sources.push(`${project}/src/POSIX/signal.c`);
    target.sources.push(`${project}/src/POSIX/thread.c`);
    target.sources.push(`${project}/src/POSIX/sleep.c`);
    target.sources.push(`${project}/src/POSIX/time.c`);
    target.sources.push(`${project}/src/POSIX/path.c`);
    target.sources.push(`${project}/src/POSIX/file.c`);
    target.sources.push(`${project}/src/POSIX/net.c`);
} else if (Deno.build.os == 'linux') {
    target.sources.push(`${project}/src/POSIX/cond.c`);
    target.sources.push(`${project}/src/POSIX/lock.c`);
    target.sources.push(`${project}/src/POSIX/signal.c`);
    target.sources.push(`${project}/src/POSIX/thread.c`);
    target.sources.push(`${project}/src/POSIX/sleep.c`);
    target.sources.push(`${project}/src/POSIX/time.c`);
    target.sources.push(`${project}/src/POSIX/path.c`);
    target.sources.push(`${project}/src/POSIX/file.c`);
    target.sources.push(`${project}/src/POSIX/net.c`);
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

const res = await target.make({ output: true, threads: cpus, log: false });
if (res.type == 'error') {
    console.log(red('Unable to build libcore'));
    Deno.exit(1);
}

if (Deno.args.includes('--test')) {
    const target = new Target();
    target.type = TargetType.executable;
    target.temp = Path.join(temp, 'test');
    target.output = Path.join(target.temp, `test${(Deno.build.os == 'windows') ? '.exe' : '.a'}`);
    target.cStandard = Standard.c17;
    target.includePath = [Path.dirname(out_inc)];
    target.libraryPath = [Path.dirname(out_lib)];
    target.libraries = ['core'];
    target.arguments.push('-fsanitize=address');
    target.debug = true;
    target.sources.push(`${project}/test/main.c`);
    if (Deno.build.os == 'windows') {

    } else if (Deno.build.os == 'darwin') {
        target.frameworks = ['Cocoa', 'Metal', 'MetalKit', 'QuartzCore'];
    }

    remove(target.temp);
    remove(target.output);

    const res = await target.make({ output: true });
    if (res.type == 'error') {
        console.log(red('Unable to build test'));
        Deno.exit();
    }
    await Deno.run({ cmd: [ target.output ] }).status();
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
