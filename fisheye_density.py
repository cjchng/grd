import numpy as np
import matplotlib.pyplot as plt


def equidistant_r(theta_rad: np.ndarray) -> np.ndarray:
    return theta_rad


def equisolid_r(theta_rad: np.ndarray) -> np.ndarray:
    return 2.0 * np.sin(theta_rad / 2.0)


def numerical_derivative(y: np.ndarray, x: np.ndarray) -> np.ndarray:
    return np.gradient(y, x)


def normalized_density(theta_rad: np.ndarray, r: np.ndarray) -> np.ndarray:
    dr_dtheta = numerical_derivative(r, theta_rad)
    sin_theta = np.sin(theta_rad)
    with np.errstate(divide="ignore", invalid="ignore"):
        density = (r * dr_dtheta) / sin_theta
    # Normalize to value at 90 degrees (theta = pi/2) for each curve
    idx_90 = np.argmin(np.abs(theta_rad - (np.pi / 2.0)))
    density_norm = density / density[idx_90]
    return density_norm


def main() -> None:
    theta_max_eqd = np.deg2rad(110.0)  # 220-degree full FOV
    theta_max_eqs = np.deg2rad(140.0)  # 280-degree full FOV

    theta_eqd = np.linspace(1e-6, theta_max_eqd, 2000)
    theta_eqs = np.linspace(1e-6, theta_max_eqs, 2000)

    r_eqd = equidistant_r(theta_eqd)
    r_eqs = equisolid_r(theta_eqs)

    dens_eqd = normalized_density(theta_eqd, r_eqd)
    dens_eqs = normalized_density(theta_eqs, r_eqs)

    # Plot r vs theta
    plt.figure(figsize=(8, 5))
    plt.plot(np.rad2deg(theta_eqd), r_eqd, label="220° Equidistant: r=θ", linewidth=2)
    plt.plot(np.rad2deg(theta_eqs), r_eqs, label="280° Equisolid: r=2sin(θ/2)", linewidth=2)
    plt.xlabel("Theta (degrees)")
    plt.ylabel("Radius r (normalized units)")
    plt.title("Fisheye Projection Radius vs Theta")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("radius_vs_theta.png", dpi=150)
    plt.close()

    # Plot normalized density vs theta
    plt.figure(figsize=(8, 5))
    plt.plot(np.rad2deg(theta_eqd), dens_eqd, label="220° Equidistant", linewidth=2)
    plt.plot(np.rad2deg(theta_eqs), dens_eqs, label="280° Equisolid", linewidth=2)
    plt.xlabel("Theta (degrees)")
    plt.ylabel(r"Normalized density $\propto r\,r'/\sin(\theta)$")
    plt.title("Normalized Pixel Density per Steradian")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("density_vs_theta.png", dpi=150)
    plt.close()

    sample_degrees = [45, 90, 110, 135, 140]

    print("Density values (normalized to each model at θ=90°):")
    print("-" * 64)
    print(f"{'Theta(deg)':>10} | {'Equidistant(220°)':>20} | {'Equisolid(280°)':>20}")
    print("-" * 64)

    for deg in sample_degrees:
        theta = np.deg2rad(deg)

        eqd_val = "N/A"
        if theta <= theta_max_eqd:
            r = equidistant_r(np.array([theta]))[0]
            dr_dtheta = 1.0
            d = (r * dr_dtheta) / np.sin(theta)
            d90 = (np.pi / 2.0) / np.sin(np.pi / 2.0)
            eqd_val = f"{(d / d90):.6f}"

        eqs_val = "N/A"
        if theta <= theta_max_eqs:
            r = equisolid_r(np.array([theta]))[0]
            dr_dtheta = np.cos(theta / 2.0)
            d = (r * dr_dtheta) / np.sin(theta)
            d90 = (
                equisolid_r(np.array([np.pi / 2.0]))[0]
                * np.cos((np.pi / 2.0) / 2.0)
                / np.sin(np.pi / 2.0)
            )
            eqs_val = f"{(d / d90):.6f}"

        print(f"{deg:10.1f} | {eqd_val:>20} | {eqs_val:>20}")

    print("\nSaved plots:")
    print("- radius_vs_theta.png")
    print("- density_vs_theta.png")


if __name__ == "__main__":
    main()
