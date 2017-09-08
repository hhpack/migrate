<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Logger;

use HHPack\Color\{Color, ForegroundColor};

final class ColorFormatter {

  public function __construct(
    private Color $debug = Color::fromColor(ForegroundColor::DarkGray),
    private Color $info = Color::fromColor(ForegroundColor::Cyan),
    private Color $warn = Color::fromColor(ForegroundColor::Yellow),
    private Color $error = Color::fromColor(ForegroundColor::Red),
  ) {}

  public function debug(string $message): string {
    return $this->debug->applyTo($message);
  }

  public function info(string $message): string {
    return $this->info->applyTo($message);
  }

  public function warn(string $message): string {
    return $this->warn->applyTo($message);
  }

  public function error(string $message): string {
    return $this->error->applyTo($message);
  }

}
